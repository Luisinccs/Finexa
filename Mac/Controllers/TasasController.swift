import Cocoa
import DcViewsMac
// import FinexaCommon - Using Bridging Header

class TasasController: NSViewController {

    // Helper to hold the shared pointer
    // Adjust type alias if needed based on actual generated C-Bridge code
    private var vmHandle: Finexa.ViewModels.TasasViewModelPtr?
    
    // MARK: - UI Controls
    
    // Grid
    private let grid = DcGrid()
    
    // Inputs
    private let cbxBase = DcComboBox()
    private let cbxDestino = DcComboBox()
    private let txtValor = DcNumberTextField()
    
    // Actions
    private let btnGuardar = NSButton(title: "Guardar Tasa", target: nil, action: nil)
    
    // Labels for layout clarity
    private let lblBase = NSTextField(labelWithString: "Moneda Base:")
    private let lblDestino = NSTextField(labelWithString: "Moneda Destino:")
    private let lblValor = NSTextField(labelWithString: "Valor:")

    // MARK: - Lifecycle
    
    deinit {
        // Handle cleanup if necessary
    }

    override func loadView() {
        self.view = NSView(frame: NSRect(x: 0, y: 0, width: 800, height: 600))
        self.view.wantsLayer = true
        self.view.layer?.backgroundColor = NSColor.windowBackgroundColor.cgColor
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        self.title = "Gestión de Tasas"
        
        setupUI()
        setupViewModel()
    }
    
    private func setupViewModel() {
        // 1. Create VM
        // Creating bindings. Assuming 'create()' is exposed via static member or global function
        vmHandle = Finexa.ViewModels.TasasViewModel.create()
        
        guard let vm = vmHandle else { return }
        
        // 2. Bind via C-Bridge
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            // Grid
            if let gridPtr = TasasViewModel_gridTasas(rawPtr) {
                print("DEBUG: Bind Grid Success")
                grid.setViewModel(gridPtr)
                // Optional: Configure column widths if needed
            } else { print("DEBUG: Bind Grid FAILED") }
            
            // Selectors
            if let basePtr = TasasViewModel_selectorBase(rawPtr) {
                cbxBase.setViewModel(basePtr)
            }
            
            if let destPtr = TasasViewModel_selectorDestino(rawPtr) {
                cbxDestino.setViewModel(destPtr)
            }
            
            // Input
            if let valPtr = TasasViewModel_inputValor(rawPtr) {
                txtValor.setViewModel(valPtr)
            }
            
            // Command
            if let savePtr = TasasViewModel_cmdGuardarTasa(rawPtr) {
                btnGuardar.setViewModel(savePtr)
            }
            
            // 3. Initialize Data
//            vm.inicializar()
//            vmPtr.pointee.inicializar()
        }
        vm.pointee.inicializar()
    }

    // MARK: - UI Setup
    
    private func setupUI() {
        // Enable AutoLayout
        let controls = [grid, cbxBase, cbxDestino, txtValor, btnGuardar, lblBase, lblDestino, lblValor]
        controls.forEach { 
            $0.translatesAutoresizingMaskIntoConstraints = false 
            self.view.addSubview($0)
        }
        
        // Button configuration
        btnGuardar.bezelStyle = .rounded
        
        // Layout Config
        let margin: CGFloat = 20
        let inputWidth: CGFloat = 150
        
        NSLayoutConstraint.activate([
            // Input Row 1: Base Currency
            lblBase.topAnchor.constraint(equalTo: self.view.topAnchor, constant: margin),
            lblBase.leadingAnchor.constraint(equalTo: self.view.leadingAnchor, constant: margin),
            lblBase.widthAnchor.constraint(equalToConstant: 100),
            
            cbxBase.centerYAnchor.constraint(equalTo: lblBase.centerYAnchor),
            cbxBase.leadingAnchor.constraint(equalTo: lblBase.trailingAnchor, constant: 10),
            cbxBase.widthAnchor.constraint(equalToConstant: inputWidth),
            
            // Input Row 2: Target Currency
            lblDestino.topAnchor.constraint(equalTo: lblBase.bottomAnchor, constant: 15),
            lblDestino.leadingAnchor.constraint(equalTo: self.view.leadingAnchor, constant: margin),
            lblDestino.widthAnchor.constraint(equalToConstant: 100),
            
            cbxDestino.centerYAnchor.constraint(equalTo: lblDestino.centerYAnchor),
            cbxDestino.leadingAnchor.constraint(equalTo: lblDestino.trailingAnchor, constant: 10),
            cbxDestino.widthAnchor.constraint(equalToConstant: inputWidth),
            
            // Input Row 3: Value
            lblValor.topAnchor.constraint(equalTo: lblDestino.bottomAnchor, constant: 15),
            lblValor.leadingAnchor.constraint(equalTo: self.view.leadingAnchor, constant: margin),
            lblValor.widthAnchor.constraint(equalToConstant: 100),
            
            txtValor.centerYAnchor.constraint(equalTo: lblValor.centerYAnchor),
            txtValor.leadingAnchor.constraint(equalTo: lblValor.trailingAnchor, constant: 10),
            txtValor.widthAnchor.constraint(equalToConstant: inputWidth),
            
            // Action Button
            btnGuardar.topAnchor.constraint(equalTo: lblValor.bottomAnchor, constant: 20),
            btnGuardar.leadingAnchor.constraint(equalTo: self.view.leadingAnchor, constant: margin),
            btnGuardar.widthAnchor.constraint(equalToConstant: 120),
            
            // Grid (Remaining space)
            grid.topAnchor.constraint(equalTo: btnGuardar.bottomAnchor, constant: margin),
            grid.leadingAnchor.constraint(equalTo: self.view.leadingAnchor, constant: margin),
            grid.trailingAnchor.constraint(equalTo: self.view.trailingAnchor, constant: -margin),
            grid.bottomAnchor.constraint(equalTo: self.view.bottomAnchor, constant: -margin)
        ])
    }
}
