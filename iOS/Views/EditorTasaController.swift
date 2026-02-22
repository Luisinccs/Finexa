import UIKit
import DcViewsIos

public class EditorTasaController: UIViewController, EditorViewHelper, EditorFormCommands {
    
    // MARK: - Properties
    private var binder: EditorTasaBinder?
    
    // MARK: - EditorFormCommands Protocol
    public let commandBar = DcCommandBar()
    public let bottomActionsView = UIView()
    public var inputFields: [UIView] { return [cmbBase, cmbDestino, txtValor] }
    
    private let btnStaticAccept = UIButton(type: .system)
    private let btnStaticCancel = UIButton(type: .system)
    
    // MARK: - UI Controls
    public let stackView = UIStackView()
    
    public let lblBase = UILabel()
    public let cmbBase = DcComboBox() // Selector
    
    public let lblDestino = UILabel()
    public let cmbDestino = DcComboBox() // Selector
    
    public let lblValor = UILabel()
    public let txtValor = DcNumberTextField() // Numérico
    
    // MARK: - Init
    
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
        setupKeyboardCommands() // Integración con EditorFormCommands
    }
    
    // MARK: - Configuration
    
    public func configure(with binder: EditorTasaBinder) {
        self.binder = binder
        loadViewIfNeeded()
        setupBindings()
    }
    
    deinit {
        binder?.unbind()
    }
    
    private func setupBindings() {
        guard let binder = binder else { return }
        binder.bind(controller: self)
    }
    
    // MARK: - EditorFormCommands Actions
    
    public func onAcceptTapped() {
        commandBar.btnAccept.sendActions(for: .touchUpInside)
    }
    
    public func onCancelTapped() {
        commandBar.btnCancel.sendActions(for: .touchUpInside)
    }
    
    // MARK: - UI Setup
    
    private func setupUI() {
        view.backgroundColor = .systemBackground
        title = "Editor Tasa"
        
        stackView.axis = .vertical
        stackView.spacing = 20
        stackView.isLayoutMarginsRelativeArrangement = true
        stackView.layoutMargins = UIEdgeInsets(top: 20, left: 20, bottom: 20, right: 20)
        
        view.addSubview(stackView)
        stackView.translatesAutoresizingMaskIntoConstraints = false
        
        NSLayoutConstraint.activate([
            stackView.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor),
            stackView.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            stackView.trailingAnchor.constraint(equalTo: view.trailingAnchor)
        ])
        
        let stackFields = UIStackView(arrangedSubviews: [
            makeFieldStack(label: lblBase, field: cmbBase),
            makeFieldStack(label: lblDestino, field: cmbDestino),
            makeFieldStack(label: lblValor, field: txtValor)
        ])
        stackFields.axis = .vertical
        stackFields.spacing = 15
        
        stackView.addArrangedSubview(stackFields)
        
        let spacer = UIView()
        stackView.addArrangedSubview(spacer)
        
        // Agregar botones estáticos del formulario en lugar de la commandBar
        setupStaticButtons()
        stackView.addArrangedSubview(bottomActionsView)
        
        // Auto focus
        _ = cmbBase.becomeFirstResponder()
    }
    
    private func setupStaticButtons() {
        btnStaticAccept.setTitle("Aceptar", for: .normal)
        btnStaticAccept.backgroundColor = .systemBlue
        btnStaticAccept.setTitleColor(.white, for: .normal)
        btnStaticAccept.layer.cornerRadius = 8
        btnStaticAccept.heightAnchor.constraint(equalToConstant: 50).isActive = true
        btnStaticAccept.addAction(UIAction { [weak self] _ in self?.onAcceptTapped() }, for: .touchUpInside)
        
        btnStaticCancel.setTitle("Cancelar", for: .normal)
        btnStaticCancel.setTitleColor(.systemRed, for: .normal)
        btnStaticCancel.heightAnchor.constraint(equalToConstant: 50).isActive = true
        btnStaticCancel.addAction(UIAction { [weak self] _ in self?.onCancelTapped() }, for: .touchUpInside)
        
        let actionsStack = UIStackView(arrangedSubviews: [btnStaticAccept, btnStaticCancel])
        actionsStack.axis = .vertical
        actionsStack.spacing = 10
        actionsStack.distribution = .fillEqually
        
        bottomActionsView.addSubview(actionsStack)
        actionsStack.translatesAutoresizingMaskIntoConstraints = false
        
        NSLayoutConstraint.activate([
            actionsStack.topAnchor.constraint(equalTo: bottomActionsView.topAnchor),
            actionsStack.bottomAnchor.constraint(equalTo: bottomActionsView.bottomAnchor),
            actionsStack.leadingAnchor.constraint(equalTo: bottomActionsView.leadingAnchor),
            actionsStack.trailingAnchor.constraint(equalTo: bottomActionsView.trailingAnchor)
        ])
    }
}
