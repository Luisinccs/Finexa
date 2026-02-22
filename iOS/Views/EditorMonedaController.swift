import UIKit
import DcViewsIos

public class EditorMonedaController: UIViewController, EditorFormCommands {
    
    // MARK: - UI Controls
    public let stackView = UIStackView()
    // MARK: - EditorFormCommands Protocol
    public let commandBar = DcCommandBar()
    public let bottomActionsView = UIView()
    public var inputFields: [UIView] { return [txtNombre, txtSimbolo, txtSiglas] }
    
    // Botones de acción estáticos
    public let cmdAceptar = UIButton()
    public let cmdCancelar = UIButton()
    
    // Campos
    public let lblNombre = UILabel()
    public let txtNombre = DcTextField()
    
    public let lblSimbolo = UILabel()
    public let txtSimbolo = DcTextField()
    
    public let lblSiglas = UILabel()
    public let txtSiglas = DcTextField()
    
    // MARK: - Dependencies
    
    /// The binder responsible for connecting UI to Data.
    /// This should be injected via `configure(with:)`.
    private var binder: EditorMonedaBinder?
    
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
        setupKeyboardCommands() // Integración con EditorFormCommands
    }
    
    // MARK: - Configuration
    
    public func configure(with binder: EditorMonedaBinder) {
        self.binder = binder
        loadViewIfNeeded()
        setupBindings()
    }
    
    deinit {
        binder?.unbind()
    }
    
    // MARK: - EditorFormCommands Actions
    
    public func onAcceptTapped() {
        cmdAceptar.sendActions(for: .touchUpInside)
    }
    
    public func onCancelTapped() {
        cmdCancelar.sendActions(for: .touchUpInside)
    }
    
    // MARK: - Callbacks
    public var onAttemptCancel: (() -> Bool)?
    
    // Legacy onCancel removed as binding handles it directly.
    // However, if we need interception, we would bind cancel manually or handle it here.
    // Assuming ViewModel-driven cancel is preferred.
    
    // MARK: - UI Setup
    
    private func setupBindings() {
        guard let binder = binder else { return }
        binder.bind(controller: self)
    }
    
    private func setupUI() {
        view.backgroundColor = .systemBackground
        title = "Editor Moneda"
        
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
        
        addField(lblNombre, txtNombre)
        addField( lblSimbolo, txtSimbolo)
        addField(lblSiglas, txtSiglas)
        
        // Botones de Acción (Bottom Actions View)
        cmdAceptar.backgroundColor = .systemBlue
        cmdAceptar.setTitleColor(.white, for: .normal)
        cmdAceptar.layer.cornerRadius = 8
        cmdAceptar.heightAnchor.constraint(equalToConstant: 50).isActive = true
        // El viewModel se conectará vía el binder a estos botones.

        cmdCancelar.setTitleColor(.systemRed, for: .normal)
        cmdCancelar.heightAnchor.constraint(equalToConstant: 50).isActive = true
        
        let actionsStack = UIStackView(arrangedSubviews: [cmdAceptar, cmdCancelar])
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
        
        // Agregar espaciador que expande y la barra de acciones
        let spacer = UIView()
        stackView.addArrangedSubview(spacer)
        stackView.addArrangedSubview(bottomActionsView)
        
        // Focus first field
        _ = txtNombre.becomeFirstResponder()
        
        // Conectar botones flotantes de commandBar a las acciones locales
        commandBar.btnAccept.addAction(UIAction { [weak self] _ in self?.onAcceptTapped() }, for: .touchUpInside)
        commandBar.btnCancel.addAction(UIAction { [weak self] _ in self?.onCancelTapped() }, for: .touchUpInside)
    }
    
    private func addField(_ label: UILabel, _ control: UIView) {

        label.font = .systemFont(ofSize: 14, weight: .medium)
        label.textColor = .secondaryLabel
        
        stackView.addArrangedSubview(label)
        stackView.addArrangedSubview(control)
        
        control.heightAnchor.constraint(equalToConstant: 44).isActive = true
    }
    
}
