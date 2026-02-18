import UIKit
import DcViewsIos

public class EditorOperacionController: UIViewController {
    
    // MARK: - Properties
    private var binder: EditorOperacionBinder?
    
    private let lblConcepto = UILabel()
    private let lblMonto = UILabel()
    private let lblMoneda = UILabel()
    private let lblMontoBase = UILabel()
    
    private let txtConcepto = UITextField()
    private let txtMonto = DcNumberTextField()
    private let cmbMoneda = DcComboBox()
    private let txtMontoBase = UITextField()
    
    private let cmdAceptar = UIButton()
    private let cmdCancelar = UIButton()
    
    public func configure(with binder: EditorOperacionBinder) {
        self.binder = binder
        loadViewIfNeeded()
        setupBindings()
    }
    
    // MARK: - Lifecycle
    
    public override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
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
        
        // Buttons Stack
        let stackButtons = UIStackView(arrangedSubviews: [cmdAceptar, cmdCancelar])
        stackButtons.axis = .vertical
        stackButtons.spacing = 10
        stackButtons.distribution = .fillEqually
        
        stackMain.addArrangedSubview(stackFields)
        stackMain.addArrangedSubview(stackButtons)
        
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
        
        // Bind UI to ViewModel via Binder
        binder.bindFields(concepto: txtConcepto, monto: txtMonto, moneda: cmbMoneda, montoRef: txtMontoBase)
        binder.bindLabels(lblConcepto: lblConcepto, lblMonto: lblMonto, lblMoneda: lblMoneda, lblMontoBase: lblMontoBase)
        binder.bindCommands(cmdAceptar: cmdAceptar, cmdCancelar: cmdCancelar)
        
        // Handle Close Request from ViewModel (e.g. after save)
        binder.bindCloseRequest { [weak self] in
            self?.dismiss(animated: true)
        }
    }
}
