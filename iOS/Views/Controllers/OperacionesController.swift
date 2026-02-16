import UIKit
import DcViewsIos

public class OperacionesController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    
    // MARK: - Properties
    private var binder: OperacionesBinder?
    private let tableView = UITableView()
    
    // Header & Footer
    private let lblReference = UILabel()
    private let lblTotal = UILabel()
    private let headerView = UIView()
    private let footerView = UIView()
    
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
        
        self.binder?.onLabelsChanged = { [weak self] (ref, total) in
            DispatchQueue.main.async {
                self?.lblReference.text = ref
                self?.lblTotal.text = total
            }
        }
        
        // Initial state
        lblReference.text = binder.referenceCurrencyTitle
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
        
        // Add Button
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
        
        lblReference.font = .systemFont(ofSize: 18, weight: .bold)
        lblReference.textColor = .label
        lblReference.textAlignment = .right
        
        headerView.addSubview(lblReference)
        lblReference.translatesAutoresizingMaskIntoConstraints = false
        
        NSLayoutConstraint.activate([
            lblReference.centerYAnchor.constraint(equalTo: headerView.centerYAnchor),
            lblReference.leadingAnchor.constraint(equalTo: headerView.leadingAnchor, constant: 20),
            lblReference.trailingAnchor.constraint(equalTo: headerView.trailingAnchor, constant: -20)
        ])
    }
    
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
        binder?.requestAdd()
        openEditForm()
    }
    
    private func openEditForm() {
        guard let editorBinder = binder?.getEditorBinder() else { return }
        
        let editVC = EditorOperacionController()
        editVC.configure(with: editorBinder)
        
        let nav = UINavigationController(rootViewController: editVC)
        nav.modalPresentationStyle = .fullScreen
        present(nav, animated: true, completion: nil)
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
        
        binder?.selectRow(at: indexPath.row)
        openEditForm()
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
