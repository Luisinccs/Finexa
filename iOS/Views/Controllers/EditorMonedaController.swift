import UIKit
// import DcViewModels // Removed to decouple
// import DcViewsIos // Removed to decouple (assuming HaloTextField is local or imported from a UI module, but if it depends on DcViewsIos for 'setViewModel', we might strictly need it only if we call it here. Wait, the Controller doesn't call setViewModel anymore! The Binder does. So we can remove this import if HaloTextField is defined here or in a UI-only module.
// However, HaloTextField is defined in this file at the bottom. It uses `setViewModel`? No, the Binder calls `setViewModel` on it. 
// Let's check HaloTextField implementation in this file. It inherits UITextField. It does NOT have `setViewModel`.
// Ah, `setViewModel` is likely an extension on UITextField provided by `DcViewsIos`.
// Since the Controller doesn't call `setViewModel`, it doesn't need `DcViewsIos` for that.
// BUT, HaloTextField might need to be moved or keep existing.
// The user said "EditorMonedaController se pueda llevar al proyecto Finexa/iOS/Views/Finexa.Views.ios.xcodeproj". 
// So it should rely on UIKit.


public class EditorMonedaController: UIViewController {
    
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
    
    // MARK: - UI Controls
    private let stackView = UIStackView()
    
    // Exposed for Binder if needed, or passed in `bindControls`
    private let lblNombre = UILabel()
    private let txtNombre = HaloTextField()
    
    private let lblSimbolo = UILabel()
    private let txtSimbolo = HaloTextField()
    
    private let lblSiglas = UILabel()
    private let txtSiglas = HaloTextField()
    
    // Toolbar Buttons
    private var btnPrev: UIBarButtonItem!
    private var btnNext: UIBarButtonItem!
    private var btnAccept: UIBarButtonItem!
    private var btnCancel: UIBarButtonItem!
    
    // MARK: - Lifecycle
    
    public override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
        setupToolbar()
        
        // If binder is already set, bind now.
        // Or we can wait for `configure` to be called.
        // If `configure` is called before viewDidLoad, we need to bind in viewDidLoad.
        // If `configure` is called after, we bind in configure.
        applyBinding()
    }
    
    // MARK: - Configuration
    
    public func configure(with binder: EditorMonedaBinder) {
        self.binder = binder
        if isViewLoaded {
            applyBinding()
        }
    }
    
    private func applyBinding() {
        guard let binder = binder else { return }
        
        // Pass controls to binder
        binder.bind(nombre: txtNombre, simbolo: txtSimbolo, siglas: txtSiglas)
    }
    
    // MARK: - Actions
    
    @objc private func onPrev() {
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
    
    @objc private func onAccept() {
        guard let binder = binder else {
            dismiss(animated: true, completion: nil)
            return
        }
        
        binder.accept()
        dismiss(animated: true, completion: nil)
    }
    
    @objc private func onCancel() {
        guard let binder = binder else {
            dismiss(animated: true, completion: nil)
            return
        }
        
        binder.cancel()
        dismiss(animated: true, completion: nil)
    }
    
    // MARK: - UI Setup
    
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
        
        // Focus first field
        _ = txtNombre.becomeFirstResponder()
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
        // Initialize with a frame to satisfy internal constraints
        let toolbar = UIToolbar(frame: CGRect(x: 0, y: 0, width: UIScreen.main.bounds.width, height: 44))
        // toolbar.sizeToFit() // Not strictly needed if frame is set, but harmless
        
        // Use SF Symbols for Tab/BackTab
        let imgPrev = UIImage(systemName: "arrow.left.to.line") ?? UIImage(systemName: "chevron.up")
        let imgNext = UIImage(systemName: "arrow.right.to.line") ?? UIImage(systemName: "chevron.down")
        
        btnPrev = UIBarButtonItem(image: imgPrev, style: .plain, target: self, action: #selector(onPrev))
        btnNext = UIBarButtonItem(image: imgNext, style: .plain, target: self, action: #selector(onNext))
        
        // "Aceptar" and "Cancelar"
        btnAccept = UIBarButtonItem(title: "Aceptar", style: .done, target: self, action: #selector(onAccept))
        btnCancel = UIBarButtonItem(title: "Cancelar", style: .plain, target: self, action: #selector(onCancel))
        btnCancel.tintColor = .systemRed
        
        let flexible = UIBarButtonItem(barButtonSystemItem: .flexibleSpace, target: nil, action: nil)
        
        // Layout: [Prev] [Next] ... [Cancel] [Accept] to keep functionality grouped
        toolbar.items = [btnPrev, btnNext, flexible, btnCancel, btnAccept]
        
        // Assign to textfields
        txtNombre.inputAccessoryView = toolbar
        txtSimbolo.inputAccessoryView = toolbar
        txtSiglas.inputAccessoryView = toolbar
    }
    
}

// MARK: - Custom Controls

public class HaloTextField: UITextField {
    
    public override init(frame: CGRect) {
        super.init(frame: frame)
        setup()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    private func setup() {
        borderStyle = .none
        backgroundColor = UIColor.secondarySystemBackground
        layer.cornerRadius = 8
        layer.borderWidth = 1
        layer.borderColor = UIColor.systemGray4.cgColor
        
        // Padding
        let paddingView = UIView(frame: CGRect(x: 0, y: 0, width: 10, height: 10))
        leftView = paddingView
        leftViewMode = .always
        rightView = paddingView
        rightViewMode = .always
    }
    
    public override func becomeFirstResponder() -> Bool {
        let result = super.becomeFirstResponder()
        if result {
            // Apply Halo
            layer.borderColor = UIColor.systemBlue.cgColor
            layer.shadowColor = UIColor.systemBlue.cgColor // Or specific cyan
            layer.shadowOpacity = 0.5
            layer.shadowOffset = .zero
            layer.shadowRadius = 4
        }
        return result
    }
    
    public override func resignFirstResponder() -> Bool {
        let result = super.resignFirstResponder()
        if result {
            // Remove Halo
            layer.borderColor = UIColor.systemGray4.cgColor
            layer.shadowOpacity = 0
        }
        return result
    }
}
