import UIKit
import DcViewsIos

public class EditorTasaController: UIViewController {
    
    // MARK: - Properties
    private var binder: EditorTasaBinder?
    
    // MARK: - UI Controls
    private let stackView = UIStackView()
    
    private let lblBase = UILabel()
    private let cmbBase = DcComboBox() // Selector
    
    private let lblDestino = UILabel()
    private let cmbDestino = DcComboBox() // Selector
    
    private let lblValor = UILabel()
    private let txtValor = DcNumberTextField() // Numérico
    
    // Toolbar
    private var btnPrev: UIBarButtonItem!
    private var btnNext: UIBarButtonItem!
    private var btnAccept: UIBarButtonItem!
    private var btnCancel: UIBarButtonItem!
    
    // MARK: - Init
    
    public init() {
        super.init(nibName: nil, bundle: nil)
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Configuration
    
    public func configure(with binder: EditorTasaBinder) {
        self.binder = binder
        if isViewLoaded {
            applyBinding()
        }
    }
    
    private func applyBinding() {
        guard let binder = binder else { return }
        binder.bind(base: cmbBase, destino: cmbDestino, valor: txtValor)
    }
    
    // MARK: - Lifecycle
    
    public override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
        setupToolbar()
        applyBinding()
    }
    
    // MARK: - Actions
    
    @objc private func onPrev() {
        if txtValor.isFirstResponder {
            _ = cmbDestino.becomeFirstResponder()
        } else if cmbDestino.isFirstResponder {
            _ = cmbBase.becomeFirstResponder()
        }
    }
    
    @objc private func onNext() {
        if cmbBase.isFirstResponder {
            _ = cmbDestino.becomeFirstResponder()
        } else if cmbDestino.isFirstResponder {
            _ = txtValor.becomeFirstResponder()
        }
    }
    
    @objc private func onAccept() {
        binder?.save()
        dismiss(animated: true, completion: nil)
    }
    
    @objc private func onCancel() {
        binder?.cancel()
        dismiss(animated: true, completion: nil)
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
        
        addField("Moneda Base", lblBase, cmbBase)
        addField("Moneda Destino", lblDestino, cmbDestino)
        addField("Tasa", lblValor, txtValor)
        
        // Auto focus
        _ = cmbBase.becomeFirstResponder()
    }
    
    private func addField(_ title: String, _ label: UILabel, _ control: UIView) {
        label.text = title
        label.font = .systemFont(ofSize: 14, weight: .medium)
        label.textColor = .secondaryLabel
        
        stackView.addArrangedSubview(label)
        stackView.addArrangedSubview(control)
        
        control.heightAnchor.constraint(equalToConstant: 44).isActive = true
    }
    
    private func setupToolbar() {
        let toolbar = UIToolbar(frame: CGRect(x: 0, y: 0, width: UIScreen.main.bounds.width, height: 44))
        
        let imgPrev = UIImage(systemName: "arrow.left.to.line") ?? UIImage(systemName: "chevron.up")
        let imgNext = UIImage(systemName: "arrow.right.to.line") ?? UIImage(systemName: "chevron.down")
        
        btnPrev = UIBarButtonItem(image: imgPrev, style: .plain, target: self, action: #selector(onPrev))
        btnNext = UIBarButtonItem(image: imgNext, style: .plain, target: self, action: #selector(onNext))
        
        btnAccept = UIBarButtonItem(title: "Aceptar", style: .done, target: self, action: #selector(onAccept))
        btnCancel = UIBarButtonItem(title: "Cancelar", style: .plain, target: self, action: #selector(onCancel))
        btnCancel.tintColor = .systemRed
        
        let flexible = UIBarButtonItem(barButtonSystemItem: .flexibleSpace, target: nil, action: nil)
        
        toolbar.items = [btnPrev, btnNext, flexible, btnCancel, btnAccept]
        
        cmbBase.inputAccessoryView = toolbar
        cmbDestino.inputAccessoryView = toolbar
        txtValor.inputAccessoryView = toolbar
    }
}
