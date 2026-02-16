import UIKit

public class TasasController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    
    // MARK: - Properties
    private var binder: TasasBinder?
    private let tableView = UITableView()
    
    // MARK: - Init
    
    public init() {
        super.init(nibName: nil, bundle: nil)
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Configuration
    
    public func configure(with binder: TasasBinder) {
        self.binder = binder
        
        self.binder?.onDataChanged = { [weak self] in
            DispatchQueue.main.async {
                self?.tableView.reloadData()
            }
        }
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
        title = "Tasas de Cambio"
        view.backgroundColor = .systemBackground
        
        // Add Button
        navigationItem.rightBarButtonItem = UIBarButtonItem(barButtonSystemItem: .add, target: self, action: #selector(onAdd))
        
        // TableView
        view.addSubview(tableView)
        tableView.translatesAutoresizingMaskIntoConstraints = false
        tableView.dataSource = self
        tableView.delegate = self
        tableView.register(TasaCell.self, forCellReuseIdentifier: "TasaCell")
        tableView.rowHeight = 60
        
        NSLayoutConstraint.activate([
            tableView.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor),
            tableView.bottomAnchor.constraint(equalTo: view.bottomAnchor),
            tableView.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            tableView.trailingAnchor.constraint(equalTo: view.trailingAnchor)
        ])
    }
    
    // MARK: - Actions
    
    @objc private func onAdd() {
        binder?.requestAdd()
        openEditForm()
    }
    
    private func openEditForm() {
        guard let editorBinder = binder?.getEditorBinder() else { return }
        
        let editVC = EditorTasaController()
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
        let cell = tableView.dequeueReusableCell(withIdentifier: "TasaCell", for: indexPath) as! TasaCell
        
        // Color de fondo alterno
        cell.backgroundColor = indexPath.row % 2 == 0 ? .systemBackground : .secondarySystemBackground
        
        if let data = binder?.cellData(for: indexPath.row) {
            cell.configure(paridad: data.paridad, valor: data.valor)
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
class TasaCell: UITableViewCell {
    
    private let lblLeft = UILabel()  // Paridad
    private let lblRight = UILabel() // Valor
    
    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        setup()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    private func setup() {
        lblLeft.font = .systemFont(ofSize: 16, weight: .medium)
        lblRight.font = .boldSystemFont(ofSize: 16)
        lblRight.textAlignment = .right
        
        let stack = UIStackView(arrangedSubviews: [lblLeft, lblRight])
        stack.axis = .horizontal
        stack.distribution = .fill
        stack.spacing = 10
        
        contentView.addSubview(stack)
        stack.translatesAutoresizingMaskIntoConstraints = false
        
        NSLayoutConstraint.activate([
            stack.centerYAnchor.constraint(equalTo: contentView.centerYAnchor),
            stack.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: 20),
            stack.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: -20)
        ])
        
        // Compression resistance to keep value visible
        lblRight.setContentCompressionResistancePriority(.required, for: .horizontal)
    }
    
    func configure(paridad: String, valor: String) {
        lblLeft.text = paridad
        lblRight.text = valor
    }
}
