import UIKit
import DcViewsIos

public class EditorTasaController: UIViewController, EditorViewHelper {
    
    // MARK: - Properties
    private var binder: EditorTasaBinder?
    
    // MARK: - UI Controls
    private let stackView = UIStackView()
    private let commandBar = DcCommandBar()
    
    private let lblBase = UILabel()
    private let cmbBase = DcComboBox() // Selector
    
    private let lblDestino = UILabel()
    private let cmbDestino = DcComboBox() // Selector
    
    private let lblValor = UILabel()
    private let txtValor = DcNumberTextField() // Numérico
    
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
        
        binder.bindFields(base: cmbBase, destino: cmbDestino, valor: txtValor)
        binder.bindLabels(lblBase: lblBase, lblDestino: lblDestino, lblValor: lblValor)
        
        binder.bindCommands(bar: commandBar)
        
        binder.bindCloseRequest { [weak self] in
            self?.dismiss(animated: true)
        }
        
        binder.bindDialog(to: self)
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
        
        stackView.addArrangedSubview(commandBar)
        
        // Auto focus
        _ = cmbBase.becomeFirstResponder()
        
        // Wire Navigation Buttons
        commandBar.btnPrev.addTarget(self, action: #selector(onPrev), for: .touchUpInside)
        commandBar.btnNext.addTarget(self, action: #selector(onNext), for: .touchUpInside)
    }
}
