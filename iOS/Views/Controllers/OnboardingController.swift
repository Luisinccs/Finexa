import UIKit

public class OnboardingController: UIViewController {
    
    public var onStart: (() -> Void)?
    public var onLoadMock: (() -> Void)?
    
    // MARK: - Lifecycle
    
    public override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
    }
    
    public override var prefersStatusBarHidden: Bool {
        return true
    }
    
    // MARK: - UI Setup
    
    private func setupUI() {
        view.backgroundColor = .systemBackground
        
        // Icon / Illustration Placeholder
        let iv = UIImageView(image: UIImage(systemName: "sparkles"))
        iv.tintColor = .systemBlue
        iv.contentMode = .scaleAspectFit
        
        // Title
        let lblTitle = UILabel()
        lblTitle.text = "Bienvenido a Finexa"
        lblTitle.font = .systemFont(ofSize: 28, weight: .bold)
        lblTitle.textAlignment = .center
        
        // Body
        let lblBody = UILabel()
        lblBody.text = "Gestiona tus finanzas de forma sencilla.\nPara comenzar, necesitamos que configures tu primera moneda."
        lblBody.font = .systemFont(ofSize: 17)
        lblBody.textColor = .secondaryLabel
        lblBody.textAlignment = .center
        lblBody.numberOfLines = 0
        
        // Button
        let btn = UIButton(type: .system)
        btn.setTitle("Configurar Moneda", for: .normal)
        btn.titleLabel?.font = .systemFont(ofSize: 18, weight: .semibold)
        btn.backgroundColor = .systemBlue
        btn.tintColor = .white
        btn.layer.cornerRadius = 25
        btn.addTarget(self, action: #selector(onStartTapped), for: .touchUpInside)
        
        // Mock Button (God Mode)
        let btnMock = UIButton(type: .system)
        btnMock.setTitle("Cargar Datos de Prueba", for: .normal)
        btnMock.titleLabel?.font = .systemFont(ofSize: 14)
        btnMock.tintColor = .systemGray
        btnMock.addTarget(self, action: #selector(onLoadMockTapped), for: .touchUpInside)
        
        let stack = UIStackView(arrangedSubviews: [iv, lblTitle, lblBody, btn, btnMock])
        stack.axis = .vertical
        stack.spacing = 25
        stack.alignment = .center
        
        view.addSubview(stack)
        stack.translatesAutoresizingMaskIntoConstraints = false
        
        NSLayoutConstraint.activate([
            stack.centerYAnchor.constraint(equalTo: view.centerYAnchor),
            stack.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: 40),
            stack.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -40),
            
            iv.heightAnchor.constraint(equalToConstant: 100),
            iv.widthAnchor.constraint(equalToConstant: 100),
            
            btn.heightAnchor.constraint(equalToConstant: 50),
            btn.widthAnchor.constraint(equalToConstant: 220)
        ])
    }
    
    @objc private func onStartTapped() {
        onStart?()
    }
    
    @objc private func onLoadMockTapped() {
        onLoadMock?()
    }
}
