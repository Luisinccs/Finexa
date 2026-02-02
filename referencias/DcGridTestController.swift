import Cocoa
import DcViewsMac
import DcViewModels

class DcGridTestController: NSViewController {

    // Helper to hold the shared pointer
//    private var vmHandle: DualComponents.TestHelpers.GridTestViewModelPtr?
    private var vmHandle: DualComponents.TestHelpers.GridTestViewModelPtr?
    
    // MARK: - UI Controls
    
    private let grid = DcGrid()
    private let lblTotal = NSTextField(labelWithString: "Total: $ 0.00")
    
    // Editors (Flyweights for binding)
    // In a real scenario, these Would be injected into the Grid cell views.
    // Here we instantiate them to bind to the VM, matching the C# Form's structure.
    private let txtQty = DcNumberTextField()
    private let cbxArt = DcComboBox()
    private let txtPrice = DcNumberTextField()
    private let chkDel = NSButton(checkboxWithTitle: "", target: nil, action: nil)
    private let dtpDate = NSDatePicker()
    
    // Actions
    private let btnSave = NSButton(title: "✔", target: nil, action: nil)
    private let btnCancel = NSButton(title: "✘", target: nil, action: nil)

    // MARK: - Lifecycle
    
    deinit {
        // vmHandle = nil
    }

    override func loadView() {
        self.view = NSView(frame: NSRect(x: 0, y: 0, width: 950, height: 600))
        self.view.wantsLayer = true
        self.view.layer?.backgroundColor = NSColor.windowBackgroundColor.cgColor
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        self.title = "Facturación (Dual Architecture - Mac)"
        
        setupUI()
        setupViewModel()
    }
    
    private func setupViewModel() {
        // 1. Create VM
        vmHandle = DualComponents.TestHelpers.GridTestViewModel.create()
        
        guard let vm = vmHandle else { return }
        
        // 2. Bind via C-Bridge
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            if let gridPtr = GridTestViewModel_grid(rawPtr) {
                grid.setViewModel(gridPtr)
                applyColumnWidths()
            }
            
            // Editors
            if let qtyPtr = GridTestViewModel_editQty(rawPtr) {
                txtQty.setViewModel(qtyPtr)
                grid.registerColumnEditor(1, txtQty)
            }
            
            if let artPtr = GridTestViewModel_editArticle(rawPtr) {
                cbxArt.setViewModel(artPtr)
                grid.registerColumnEditor(2, cbxArt)
            }
            
            if let pricePtr = GridTestViewModel_editPrice(rawPtr) {
                txtPrice.setViewModel(pricePtr)
                grid.registerColumnEditor(3, txtPrice)
            }
            
            if let delPtr = GridTestViewModel_editDelivered(rawPtr) {
                chkDel.setViewModel(delPtr)
                grid.registerColumnEditor(4, chkDel)
            }
            
            if let datePtr = GridTestViewModel_editDate(rawPtr) {
                dtpDate.setViewModel(datePtr)
                grid.registerColumnEditor(5, dtpDate)
            }
            
            // Actions
            if let savePtr = GridTestViewModel_cmdSave(rawPtr) {
                btnSave.setViewModel(savePtr)
            }
            
            if let cancelPtr = GridTestViewModel_cmdCancel(rawPtr) {
                btnCancel.setViewModel(cancelPtr)
            }
            grid.registerActionButtons(btnSave, btnCancel)
            
            // Footer
            if let lblPtr = GridTestViewModel_lblTotal(rawPtr) {
                lblTotal.setViewModel(lblPtr) // Note: Using setViewModel(IControlBinding) for labels usually
            }
        }
    }
    

    private func applyColumnWidths() {
        guard let tableView = grid.documentView as? NSTableView else { return }
        
        // Widths corresponding to: #, Cant, Descripción, Precio, Entr?, F. Entrega, Subtotal, Op
        // Increased Op width to 100 for proper button visibility
        let widths: [CGFloat] = [30, 40, 250, 80, 50, 100, 80, 100]
        
        for (i, width) in widths.enumerated() {
            if i < tableView.tableColumns.count {
                tableView.tableColumns[i].width = width
            }
        }
    }

    // MARK: - UI Setup
    
    private func setupUI() {
        grid.translatesAutoresizingMaskIntoConstraints = false
        lblTotal.translatesAutoresizingMaskIntoConstraints = false
        
        // Configure Action Buttons with Icons
        btnSave.bezelStyle = .inline
//        btnSave.image = NSImage(named: NSImage.menuOnStateTemplate)
        btnSave.image = NSImage(named: NSImage.folderName)
        btnSave.title = ""
        
        btnCancel.bezelStyle = .inline
        btnSave.image = NSImage(named: NSImage.trashFullName)
        btnCancel.title = ""
        
        self.view.addSubview(grid)
        self.view.addSubview(lblTotal)
        
        // Style Total Label
        lblTotal.font = NSFont.systemFont(ofSize: 18, weight: .bold)
        lblTotal.textColor = NSColor.darkBlue // Custom color or system blue
        lblTotal.alignment = .right
        
        NSLayoutConstraint.activate([
            // Grid (Top, full width, fixed height like C#)
            grid.topAnchor.constraint(equalTo: self.view.topAnchor, constant: 20),
            grid.leadingAnchor.constraint(equalTo: self.view.leadingAnchor, constant: 20),
            grid.trailingAnchor.constraint(equalTo: self.view.trailingAnchor, constant: -20),
            grid.heightAnchor.constraint(equalToConstant: 400),
            
            // Total Label (Bottom Right)
            lblTotal.topAnchor.constraint(equalTo: grid.bottomAnchor, constant: 20),
            lblTotal.trailingAnchor.constraint(equalTo: self.view.trailingAnchor, constant: -20),
            lblTotal.widthAnchor.constraint(equalToConstant: 300)
        ])
    }
}

// Color helper
extension NSColor {
    static let darkBlue = NSColor(srgbRed: 0, green: 0, blue: 0.5, alpha: 1.0)
}
