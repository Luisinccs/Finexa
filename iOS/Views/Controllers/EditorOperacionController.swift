import UIKit
import DcViewsIos

public class EditorOperacionController: UIViewController {
    
    // MARK: - Properties
    private var binder: EditorOperacionBinder?
    
    private let txtConcepto = UITextField()
    private let txtMonto = DcNumberTextField()
    private let cmbMoneda = DcComboBox()
    private let lblMontoRef = UILabel()
    
    // MARK: - Configuration
    
    public func configure(with binder: EditorOperacionBinder) {
        self.binder = binder
    }
    
    // MARK: - Lifecycle
    
    public override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
        setupBindings()
    }
    
    // MARK: - UI Setup
    
    private func setupUI() {
        title = "Editor Operación"
        view.backgroundColor = .systemBackground
        
        navigationItem.leftBarButtonItem = UIBarButtonItem(barButtonSystemItem: .cancel, target: self, action: #selector(onCancel))
        navigationItem.rightBarButtonItem = UIBarButtonItem(barButtonSystemItem: .save, target: self, action: #selector(onSave))
        
        // Styles
        txtConcepto.borderStyle = .roundedRect
        txtConcepto.placeholder = "Concepto"
        
        txtMonto.borderStyle = .roundedRect
        txtMonto.placeholder = "Monto"
        
        // Label Style
        lblMontoRef.font = .boldSystemFont(ofSize: 16)
        lblMontoRef.textColor = .secondaryLabel
        lblMontoRef.textAlignment = .right
        lblMontoRef.text = "REF: 0.00"
        
        let stack = UIStackView(arrangedSubviews: [
            label("Concepto"), txtConcepto,
            label("Monto"), txtMonto,
            label("Moneda"), cmbMoneda,
            lblMontoRef
        ])
        stack.axis = .vertical
        stack.spacing = 15
        
        view.addSubview(stack)
        stack.translatesAutoresizingMaskIntoConstraints = false
        
        NSLayoutConstraint.activate([
            stack.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: 20),
            stack.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: 20),
            stack.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -20),
            
            cmbMoneda.heightAnchor.constraint(equalToConstant: 44)
        ])
    }
    
    private func label(_ text: String) -> UILabel {
        let l = UILabel()
        l.text = text
        l.font = .systemFont(ofSize: 14, weight: .semibold)
        return l
    }
    
    private func setupBindings() {
        // Bind UI to ViewModel via Binder
        binder?.bind(concepto: txtConcepto, monto: txtMonto, moneda: cmbMoneda, montoRef: lblMontoRef)
    }
    
    // MARK: - Actions
    
    @objc private func onSave() {
        binder?.save()
        dismiss(animated: true, completion: nil)
    }
    
    @objc private func onCancel() {
        binder?.cancel()
        dismiss(animated: true, completion: nil)
    }
}
