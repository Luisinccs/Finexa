import UIKit
import DcViewsIos

public class EditorOperacionController: UIViewController, EditorFormCommands {
    
    // MARK: - Properties
    private var binder: EditorOperacionBinder?
    
    // MARK: - EditorFormCommands Protocol
    public let commandBar = DcCommandBar()
    
    public let bottomActionsView = UIView()
    public var inputFields: [UIView] { return [txtConcepto, txtMonto, cmbMoneda] }
    
    public let lblConcepto = UILabel()
    public let lblMonto = UILabel()
    public let lblMoneda = UILabel()
    public let lblMontoBase = UILabel()
    
    public let txtConcepto = UITextField()
    public let txtMonto = DcNumberTextField()
    public let cmbMoneda = DcComboBox()
    public let txtMontoBase = UITextField()
    
    public let cmdAceptar = UIButton()
    public let cmdCancelar = UIButton()
    
    public func configure(with binder: EditorOperacionBinder) {
        self.binder = binder
        loadViewIfNeeded()
        setupBindings()
    }
    
    deinit {
        binder?.unbind()
    }
    
    // MARK: - Lifecycle
    
    public override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
        setupKeyboardCommands() // Integración con EditorFormCommands
    }
    
    // MARK: - EditorFormCommands Actions
    
    public func onAcceptTapped() {
        cmdAceptar.sendActions(for: .touchUpInside)
    }
    
    public func onCancelTapped() {
        cmdCancelar.sendActions(for: .touchUpInside)
    }
    
    // MARK: - UI Setup
    
    private func setupUI() {
        title = "Operación"
        view.backgroundColor = .systemBackground
        
        // Styles
        txtConcepto.borderStyle = .roundedRect
        txtMonto.borderStyle = .roundedRect
        txtMontoBase.borderStyle = .roundedRect
        txtMontoBase.isEnabled = false // Read-only but keep styling
        txtMontoBase.textColor = .secondaryLabel
        
        // Buttons
        cmdAceptar.backgroundColor = .systemBlue
        cmdAceptar.setTitleColor(.white, for: .normal)
        cmdAceptar.layer.cornerRadius = 8
        
        cmdCancelar.setTitleColor(.systemRed, for: .normal)

        // Layout Construction
        let stackMain = UIStackView()
        stackMain.axis = .vertical
        stackMain.spacing = 20
        stackMain.translatesAutoresizingMaskIntoConstraints = false
        
        // Fileds Stack
        let stackFields = UIStackView(arrangedSubviews: [
            makeFieldStack(label: lblConcepto, field: txtConcepto),
            makeFieldStack(label: lblMonto, field: txtMonto),
            makeFieldStack(label: lblMoneda, field: cmbMoneda),
            makeFieldStack(label: lblMontoBase, field: txtMontoBase)
        ])
        stackFields.axis = .vertical
        stackFields.spacing = 15
        
        // Buttons Stack (Bottom Actions)
        let stackButtons = UIStackView(arrangedSubviews: [cmdAceptar, cmdCancelar])
        stackButtons.axis = .vertical
        stackButtons.spacing = 10
        stackButtons.distribution = .fillEqually
        
        bottomActionsView.addSubview(stackButtons)
        stackButtons.translatesAutoresizingMaskIntoConstraints = false
        
        NSLayoutConstraint.activate([
            stackButtons.topAnchor.constraint(equalTo: bottomActionsView.topAnchor),
            stackButtons.bottomAnchor.constraint(equalTo: bottomActionsView.bottomAnchor),
            stackButtons.leadingAnchor.constraint(equalTo: bottomActionsView.leadingAnchor),
            stackButtons.trailingAnchor.constraint(equalTo: bottomActionsView.trailingAnchor)
        ])
        
        stackMain.addArrangedSubview(stackFields)
        stackMain.addArrangedSubview(bottomActionsView)
        
        // Sincronizar commandBar con los botones estáticos a los cuales el Binder está conectado
        commandBar.btnAccept.addAction(UIAction { [weak self] _ in self?.onAcceptTapped() }, for: .touchUpInside)
        commandBar.btnCancel.addAction(UIAction { [weak self] _ in self?.onCancelTapped() }, for: .touchUpInside)
        
        view.addSubview(stackMain)
        
        NSLayoutConstraint.activate([
            stackMain.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: 20),
            stackMain.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: 20),
            stackMain.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -20),
            
            cmbMoneda.heightAnchor.constraint(equalToConstant: 44),
            cmdAceptar.heightAnchor.constraint(equalToConstant: 50),
            cmdCancelar.heightAnchor.constraint(equalToConstant: 50)
        ])
    }
    
    private func makeFieldStack(label: UILabel, field: UIView) -> UIStackView {
        label.font = .systemFont(ofSize: 14, weight: .semibold)
        label.textColor = .label
        
        let stack = UIStackView(arrangedSubviews: [label, field])
        stack.axis = .vertical
        stack.spacing = 5
        return stack
    }
    
    private func setupBindings() {
        guard let binder = binder else { return }
        binder.bind(controller: self)
    }
}
