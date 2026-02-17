import UIKit
import DcViewsIos

public class OperacionesController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    
    // MARK: - Properties
    private var binder: OperacionesBinder?
    private let tableView = UITableView()
    
    // Header & Footer
    private let cmbReference = DcComboBox()
    private let lblTotal = UILabel()
    private let headerView = UIView()
    private let footerView = UIView()
    
    /// Closure invoked when a menu option is selected.
    /// Key values: "tasas", "monedas"
    public var onMenuOptionSelected: ((String) -> Void)?
    
    // MARK: - Init
    
    public init() {
        super.init(nibName: nil, bundle: nil)
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Configuration
    
    public func configure(with binder: OperacionesBinder) {
        self.binder = binder
        
        self.binder?.onDataChanged = { [weak self] in
            DispatchQueue.main.async {
                self?.tableView.reloadData()
            }
        }
        
        self.binder?.onTotalChanged = { [weak self] total in
            DispatchQueue.main.async {
                self?.lblTotal.text = total
            }
        }
        
        // Bind ComboBox de moneda referencial
        binder.bindRefCurrencySelector(cmbReference)
        
        // Initial state
        lblTotal.text = binder.totalAmountTitle
    }
    
    // MARK: - Lifecycle
    
    public override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
        
        // Initial Load
        tableView.reloadData()
    }
    
    public override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        tableView.reloadData()
    }
    
    // MARK: - UI Setup
    
    private func setupUI() {
        title = "Operaciones"
        view.backgroundColor = .systemBackground
        
        // Hamburger Menu Button (left)
        let menuImage = UIImage(systemName: "line.3.horizontal")
        navigationItem.leftBarButtonItem = UIBarButtonItem(image: menuImage, style: .plain, target: self, action: #selector(onMenuTapped))
        
        // Add Button (right)
        navigationItem.rightBarButtonItem = UIBarButtonItem(barButtonSystemItem: .add, target: self, action: #selector(onAdd))
        
        // TableView
        tableView.dataSource = self
        tableView.delegate = self
        tableView.register(OperacionCell.self, forCellReuseIdentifier: "OperacionCell")
        tableView.rowHeight = 70
        
        // Layout
        setupHeader()
        setupFooter()
        
        let stackView = UIStackView(arrangedSubviews: [headerView, tableView, footerView])
        stackView.axis = .vertical
        stackView.spacing = 0
        
        view.addSubview(stackView)
        stackView.translatesAutoresizingMaskIntoConstraints = false
        
        NSLayoutConstraint.activate([
            headerView.heightAnchor.constraint(equalToConstant: 50),
            footerView.heightAnchor.constraint(equalToConstant: 60),
            
            stackView.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor),
            stackView.bottomAnchor.constraint(equalTo: view.bottomAnchor),
            stackView.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            stackView.trailingAnchor.constraint(equalTo: view.trailingAnchor)
        ])
    }
    
    private func setupHeader() {
        headerView.backgroundColor = UIColor.systemBlue.withAlphaComponent(0.1)
        
        let lblTitle = UILabel()
        lblTitle.text = "Moneda Base:" // Renamed from Moneda Ref
        lblTitle.font = .systemFont(ofSize: 14, weight: .semibold)
        lblTitle.textColor = .secondaryLabel
        lblTitle.setContentHuggingPriority(.required, for: .horizontal)
        
        cmbReference.font = .systemFont(ofSize: 16, weight: .bold)
        
        let hStack = UIStackView(arrangedSubviews: [lblTitle, cmbReference])
        hStack.axis = .horizontal
        hStack.spacing = 8
        hStack.alignment = .center
        
        headerView.addSubview(hStack)
        hStack.translatesAutoresizingMaskIntoConstraints = false
        
        NSLayoutConstraint.activate([
            hStack.centerYAnchor.constraint(equalTo: headerView.centerYAnchor),
            hStack.leadingAnchor.constraint(equalTo: headerView.leadingAnchor, constant: 16),
            hStack.trailingAnchor.constraint(equalTo: headerView.trailingAnchor, constant: -16)
        ])
    }
    
    // ... (rest of setupFooter and methods)

    // MARK: - Empty State & Coach Marks
    
    private var coachMarkView: UIView?
    
    private func checkEmptyState() {
        guard let binder = binder else { return }
        
        // 1. Check Currencies
        if !binder.hasCurrencies {
            // Welcome & Redirect
            let alert = UIAlertController(title: "Bienvenido a Finexa", message: "Para comenzar a registrar operaciones, primero debes crear al menos una moneda.", preferredStyle: .alert)
            alert.addAction(UIAlertAction(title: "Ir a Monedas", style: .default) { [weak self] _ in
                self?.onMenuOptionSelected?("monedas_empty")
            })
            present(alert, animated: true)
            return
        }
        
        // 2. Check Operations
        if binder.numberOfRows() == 0 {
            lblTotal.isHidden = true
            
            if !binder.isRefCurrencySelected {
                showCoachMark(message: "👆 Selecciona una Moneda Base para comenzar", target: cmbReference)
            } else {
                 // Target Add Button (Right Bar Button Item)
                 // We need a view for the bar button item.
                 if let view = navigationItem.rightBarButtonItem?.value(forKey: "view") as? UIView {
                     showCoachMark(message: "👆 Agrega tu primera operación", target: view)
                 } else {
                     // Fallback if view not found (unlikely but possible)
                     showCoachMark(message: "👆 Agrega tu primera operación (Botón +)", target: cmbReference)
                 }
            }
        } else {
            lblTotal.isHidden = false
            hideCoachMark()
        }
        
        // Additional Coach Mark logic for "Add Operation" could go here
        // If Base is selected (how to check? combo has selection? 
        // We might need to listen to combo changes or bind it)
        // For now, simpler empty state is implemented.
    }
    
    private func showCoachMark(message: String, target: UIView) {
        hideCoachMark()
        
        let container = UIView()
        container.backgroundColor = UIColor.systemYellow
        container.layer.cornerRadius = 8
        container.layer.shadowColor = UIColor.black.cgColor
        container.layer.shadowOpacity = 0.2
        container.layer.shadowOffset = CGSize(width: 0, height: 2)
        
        let lbl = UILabel()
        lbl.text = message
        lbl.font = .systemFont(ofSize: 14, weight: .medium)
        lbl.textColor = .black
        lbl.numberOfLines = 0
        
        container.addSubview(lbl)
        lbl.translatesAutoresizingMaskIntoConstraints = false
        
        view.addSubview(container)
        container.translatesAutoresizingMaskIntoConstraints = false
        self.coachMarkView = container
        
        NSLayoutConstraint.activate([
            lbl.topAnchor.constraint(equalTo: container.topAnchor, constant: 8),
            lbl.bottomAnchor.constraint(equalTo: container.bottomAnchor, constant: -8),
            lbl.leadingAnchor.constraint(equalTo: container.leadingAnchor, constant: 12),
            lbl.trailingAnchor.constraint(equalTo: container.trailingAnchor, constant: -12),
            
            container.topAnchor.constraint(equalTo: target.bottomAnchor, constant: 8),
            container.leadingAnchor.constraint(equalTo: target.leadingAnchor),
            container.trailingAnchor.constraint(lessThanOrEqualTo: view.trailingAnchor, constant: -20)
        ])
        
        // Animation
        container.transform = CGAffineTransform(translationX: 0, y: -10)
        container.alpha = 0
        UIView.animate(withDuration: 0.3) {
            container.transform = .identity
            container.alpha = 1
        }
    }
    
    private func hideCoachMark() {
        coachMarkView?.removeFromSuperview()
        coachMarkView = nil
    }

    public override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        checkEmptyState()
    }
    
    // ...
    
    private func setupFooter() {
        footerView.backgroundColor = UIColor.systemGreen.withAlphaComponent(0.1)
        
        lblTotal.font = .systemFont(ofSize: 20, weight: .heavy)
        lblTotal.textColor = .label
        lblTotal.textAlignment = .right
        
        footerView.addSubview(lblTotal)
        lblTotal.translatesAutoresizingMaskIntoConstraints = false
        
        NSLayoutConstraint.activate([
            lblTotal.centerYAnchor.constraint(equalTo: footerView.centerYAnchor),
            lblTotal.leadingAnchor.constraint(equalTo: footerView.leadingAnchor, constant: 20),
            lblTotal.trailingAnchor.constraint(equalTo: footerView.trailingAnchor, constant: -20)
        ])
    }
    
    // MARK: - Actions
    
    @objc private func onAdd() {
        openEditForm(thenPrepareNew: true)
    }
    
    @objc private func onMenuTapped() {
        let alert = UIAlertController(title: "Menú", message: nil, preferredStyle: .actionSheet)
        
        alert.addAction(UIAlertAction(title: "💱 Tasas de Cambio", style: .default) { [weak self] _ in
            self?.onMenuOptionSelected?("tasas")
        })
        
        alert.addAction(UIAlertAction(title: "💰 Monedas", style: .default) { [weak self] _ in
            self?.onMenuOptionSelected?("monedas")
        })
        
        // God Mode
        alert.addAction(UIAlertAction(title: "🛠️ Cargar Mock (God Mode)", style: .destructive) { [weak self] _ in
            self?.binder?.cargarMock()
        })
        
        alert.addAction(UIAlertAction(title: "💣 Limpiar DB (God Mode)", style: .destructive) { [weak self] _ in
             self?.binder?.limpiarDB()
        })
        
        alert.addAction(UIAlertAction(title: "Cancelar", style: .cancel))
        
        // iPad popover anchor
        if let popover = alert.popoverPresentationController {
            popover.barButtonItem = navigationItem.leftBarButtonItem
        }
        
        present(alert, animated: true)
    }
    
    private func openEditForm(thenPrepareNew: Bool = false) {
        guard let editorBinder = binder?.getEditorBinder() else { return }
        
        let editVC = EditorOperacionController()
        editVC.configure(with: editorBinder)
        
        let nav = UINavigationController(rootViewController: editVC)
        nav.modalPresentationStyle = .fullScreen
        
        // Forzar viewDidLoad → setupBindings() para que los callbacks C++
        // apunten a los nuevos controles ANTES de llamar requestAdd()
        editVC.loadViewIfNeeded()
        
        present(nav, animated: true) { [weak self] in
            if thenPrepareNew {
                self?.binder?.requestAdd()
            }
        }
    }
    
    // MARK: - UITableViewDataSource
    
    public func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return binder?.numberOfRows() ?? 0
    }
    
    public func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "OperacionCell", for: indexPath) as! OperacionCell
        
        // Color de fondo alterno
        cell.backgroundColor = indexPath.row % 2 == 0 ? .systemBackground : .secondarySystemBackground
        
        if let data = binder?.cellData(for: indexPath.row) {
            cell.configure(concepto: data.concepto, monto: data.monto, moneda: data.moneda, montoXds: data.montoXds)
        }
        
        return cell
    }
    
    // MARK: - UITableViewDelegate
    
    public func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        tableView.deselectRow(at: indexPath, animated: true)
        
        let row = indexPath.row
        guard let editorBinder = binder?.getEditorBinder() else { return }
        
        let editVC = EditorOperacionController()
        editVC.configure(with: editorBinder)
        
        let nav = UINavigationController(rootViewController: editVC)
        nav.modalPresentationStyle = .fullScreen
        
        editVC.loadViewIfNeeded()
        
        present(nav, animated: true) { [weak self] in
            self?.binder?.selectRow(at: row)
        }
    }
    
    public func tableView(_ tableView: UITableView, trailingSwipeActionsConfigurationForRowAt indexPath: IndexPath) -> UISwipeActionsConfiguration? {
        let deleteAction = UIContextualAction(style: .destructive, title: "Eliminar") { [weak self] (_, _, completion) in
             self?.binder?.selectRow(at: indexPath.row)
             self?.binder?.deleteRow(at: indexPath.row)
             completion(true)
        }
        return UISwipeActionsConfiguration(actions: [deleteAction])
    }
}

// MARK: - Custom Cell
class OperacionCell: UITableViewCell {
    
    private let lblConcepto = UILabel()
    private let lblMonto = UILabel()
    
    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        setup()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    private func setup() {
        lblConcepto.font = .systemFont(ofSize: 16, weight: .medium)
        lblConcepto.numberOfLines = 2
        
        lblMonto.font = .boldSystemFont(ofSize: 16)
        lblMonto.textAlignment = .right
        
        let stack = UIStackView(arrangedSubviews: [lblConcepto, lblMonto])
        stack.axis = .horizontal
        stack.distribution = .fill
        stack.alignment = .center
        stack.spacing = 10
        
        contentView.addSubview(stack)
        stack.translatesAutoresizingMaskIntoConstraints = false
        
        NSLayoutConstraint.activate([
            stack.topAnchor.constraint(equalTo: contentView.topAnchor, constant: 5),
            stack.bottomAnchor.constraint(equalTo: contentView.bottomAnchor, constant: -5),
            stack.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: 15),
            stack.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: -15)
        ])
        
        // Compression
        lblMonto.setContentCompressionResistancePriority(.required, for: .horizontal)
    }
    
    func configure(concepto: String, monto: String, moneda: String, montoXds: String) {
        lblConcepto.text = concepto
        // Format: $100.00 USD
        // Or if we want to show conversion: $100.00 USD\n($50.00 REF)
        // Let's use attributed text or simple format
        
        let montoStr = "\(monto) \(moneda)"
        lblMonto.text = montoStr
        
        // Maybe detail text for XDS?
        // For simplicity, just original amount. The prompt mentioned "Monto" and "MontoEnMonedaRef" in editor,
        // but for grid row usually one line.
        // The prompt says "parte de abajo se refleja el monto total".
    }
}
