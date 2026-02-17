import UIKit

public class MonedasController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    
    // MARK: - Dependencies
    private var binder: MonedasBinder?
    private let tableView = UITableView()
    
    // MARK: - Navigation Callbacks
    public var onBackToOperations: (() -> Void)?
    public var autoOpenEditor = false
    private var wasEmpty = false
    
    // MARK: - Initializers
    
    public init() {
        super.init(nibName: nil, bundle: nil)
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Lifecycle
    
    public override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
        
        // Hide table initially to avoid flash if auto-opening editor
        if autoOpenEditor {
            tableView.isHidden = true
        }
    }
    
    public override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        if autoOpenEditor {
            autoOpenEditor = false
            onAdd()
        } else {
            // Unhide when appearing normally or returning
            tableView.isHidden = false
        }
    }
    
    public override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        tableView.reloadData()
        
        // Check outcome of Add/Edit
        if wasEmpty {
            let count = binder?.numberOfRows() ?? 0
            if count > 0 {
                // Success: Return to Operations
                wasEmpty = false
                tableView.isHidden = true // Hide while transitioning
                DispatchQueue.main.asyncAfter(deadline: .now() + 0.5) { [weak self] in
                    self?.onBackToOperations?()
                }
            } else {
                // Cancelled / No currency created
                // Determine if we should unhide or waiting for editor logic
                // If editor was dismissed (which logic prevents now), we unhide.
                tableView.isHidden = false
                wasEmpty = false 
                
                let alert = UIAlertController(title: "Atención", message: "Para registrar operaciones, necesitas crear al menos una moneda.", preferredStyle: .alert)
                alert.addAction(UIAlertAction(title: "Crear Moneda", style: .default) { [weak self] _ in
                    self?.onAdd()
                })
                alert.addAction(UIAlertAction(title: "Cancelar", style: .cancel, handler: nil))
                
                // Present alert only if we are the top controller
                if presentedViewController == nil {
                    present(alert, animated: true)
                }
            }
        }
    }
    
    // ...
    
    // MARK: - Actions
    
    @objc private func onAdd() {
        // Track if empty before adding
        wasEmpty = (binder?.numberOfRows() ?? 0) == 0
        
        binder?.requestAdd()
        openEditForm()
    }
    
    private func openEditForm() {
        guard let editorBinder = binder?.getEditorBinder() else { return }
        
        let editVC = EditorMonedaController()
        editVC.configure(with: editorBinder)
        
        // Prevent swipe dismissal if empty
        if (binder?.numberOfRows() ?? 0) == 0 {
            editVC.isModalInPresentation = true
            
            // Intercept Cancel
            editVC.onAttemptCancel = { [weak editVC] in
                let alert = UIAlertController(title: "Atención", message: "Para registrar operaciones, necesitas crear al menos una moneda.", preferredStyle: .alert)
                alert.addAction(UIAlertAction(title: "Entendido", style: .default, handler: nil))
                
                editVC?.present(alert, animated: true, completion: nil)
                
                // Return false to prevent dismissal
                return false
            }
        }
        
        let nav = UINavigationController(rootViewController: editVC)
        nav.modalPresentationStyle = .fullScreen
        // Callback when dismissed? Editor dismisses itself.
        // viewWillAppear will handle the check.
        present(nav, animated: true, completion: nil)
    }
    
    public func configure(with binder: MonedasBinder) {
        self.binder = binder
        self.binder?.onDataChanged = { [weak self] in
            self?.tableView.reloadData()
        }
        if isViewLoaded {
            tableView.reloadData()
        }
    }
    
    // MARK: - UI Setup
    
    private func setupUI() {
        title = "Monedas"
        view.backgroundColor = .systemBackground
        
        // Add Button
        navigationItem.rightBarButtonItem = UIBarButtonItem(barButtonSystemItem: .add, target: self, action: #selector(onAdd))
        
        // TableView
        view.addSubview(tableView)
        tableView.translatesAutoresizingMaskIntoConstraints = false
        tableView.dataSource = self
        tableView.delegate = self
        tableView.register(MonedaCell.self, forCellReuseIdentifier: "Cell")
        tableView.rowHeight = 60
        
        NSLayoutConstraint.activate([
            tableView.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor),
            tableView.bottomAnchor.constraint(equalTo: view.bottomAnchor),
            tableView.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            tableView.trailingAnchor.constraint(equalTo: view.trailingAnchor)
        ])
    }
    
    
    // MARK: - UITableViewDataSource
    
    public func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return binder?.numberOfRows() ?? 0
    }
    
    public func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "Cell", for: indexPath) as! MonedaCell
        
        cell.backgroundColor = indexPath.row % 2 == 0 ? .systemBackground : .secondarySystemBackground
        
        if let data = binder?.cellData(for: indexPath.row) {
            cell.configure(nombre: data.nombre, simbolo: data.simbolo, siglas: data.siglas)
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
             self?.binder?.deleteRow(at: indexPath.row)
             completion(true)
        }
        return UISwipeActionsConfiguration(actions: [deleteAction])
    }
}

// MARK: - Custom Cell
public class MonedaCell: UITableViewCell {
    
    private let lblLeft = UILabel()  // Nombre (Simbolo)
    private let lblRight = UILabel() // Siglas
    
    public override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
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
        
        lblRight.setContentCompressionResistancePriority(.required, for: .horizontal)
    }
    
    public func configure(nombre: String, simbolo: String, siglas: String) {
        if nombre.isEmpty {
            lblLeft.text = ""
            lblRight.text = ""
            return
        }
        lblLeft.text = "\(nombre) (\(simbolo))"
        lblRight.text = siglas
    }
}

