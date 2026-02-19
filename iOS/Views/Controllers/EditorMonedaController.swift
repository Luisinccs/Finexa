import UIKit
import DcViewsIos

public class EditorMonedaController: UIViewController {
    
    // MARK: - UI Controls
    private let stackView = UIStackView()
    private let commandBar = DcCommandBar()
    
    // Campos
    private let lblNombre = UILabel()
    private let txtNombre = DcTextField()
    
    private let lblSimbolo = UILabel()
    private let txtSimbolo = DcTextField()
    
    private let lblSiglas = UILabel()
    private let txtSiglas = DcTextField()
    
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
    }
    
    // MARK: - Configuration
    
    public func configure(with binder: EditorMonedaBinder) {
        self.binder = binder
        loadViewIfNeeded()
        setupBindings()
    }
    
    // MARK: - Actions
    
    @objc private func onPrev() { // Keeping method name but target connects from viewDidLoad
        // Simple focus logic
        if txtSiglas.isFirstResponder {
            _ = txtSimbolo.becomeFirstResponder()
        } else if txtSimbolo.isFirstResponder {
            _ = txtNombre.becomeFirstResponder()
        }
    }
    
    @objc private func onNext() {
        if txtNombre.isFirstResponder {
            _ = txtSimbolo.becomeFirstResponder()
        } else if txtSimbolo.isFirstResponder {
            _ = txtSiglas.becomeFirstResponder()
        }
    }
    
    // MARK: - Callbacks
    public var onAttemptCancel: (() -> Bool)?
    
    // Legacy onCancel removed as binding handles it directly.
    // However, if we need interception, we would bind cancel manually or handle it here.
    // Assuming ViewModel-driven cancel is preferred.
    
    // MARK: - UI Setup
    
    private func setupBindings(){
        guard let binder = binder else { return }
        binder.bindFields(nombre: txtNombre, simbolo: txtSimbolo, siglas: txtSiglas)
        binder.bindLabels(lblNombre: lblNombre, lblSimbolo: lblSimbolo, lblSiglas: lblSiglas)
        
        binder.bindCommands(bar: commandBar) // Updated to new signature
        
        // Handle Close Request from ViewModel (e.g. after save)
        binder.bindCloseRequest { [weak self] in
            self?.dismiss(animated: true)
        }
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
        
        addField("Nombre", lblNombre, txtNombre)
        addField("Símbolo", lblSimbolo, txtSimbolo)
        addField("Siglas", lblSiglas, txtSiglas)
        
        // Spacer to push command bar to bottom (optional, but good for stack)
        let spacer = UIView()
        stackView.addArrangedSubview(spacer)
        
        // Add Command Bar
        stackView.addArrangedSubview(commandBar)
        
        // Focus first field
        _ = txtNombre.becomeFirstResponder()
        
        // Wire Navigation Buttons
        commandBar.btnPrev.addTarget(self, action: #selector(onPrev), for: .touchUpInside)
        commandBar.btnNext.addTarget(self, action: #selector(onNext), for: .touchUpInside)
    }
    
    private func addField(_ title: String, _ label: UILabel, _ control: UIView) {
        label.text = title
        label.font = .systemFont(ofSize: 14, weight: .medium)
        label.textColor = .secondaryLabel
        
        stackView.addArrangedSubview(label)
        stackView.addArrangedSubview(control)
        
        control.heightAnchor.constraint(equalToConstant: 44).isActive = true
    }
    
}

// MARK: - Custom Controls
// HaloTextField removed - replaced by DcTextField in DcViewsIos

