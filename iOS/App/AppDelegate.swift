//  2026/02/15

import UIKit
import FinexaViewsIos

@main
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        // 0. Inicializar Base de Datos
        let documentsPath = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true).first!
        let dbPath = (documentsPath as NSString).appendingPathComponent("finexa.db")
        Finexa_DatabaseInitializer_setup(dbPath)
        
        // 1. Configuración de Ventana
        window = UIWindow(frame: UIScreen.main.bounds)
        
        // 2. Determinar flujo Inicial
        let binder = OperacionesAppBinder()
        if !binder.hasCurrencies {
            showOnboarding(with: binder)
        } else {
            showMainApp(with: binder)
        }
        
        window?.makeKeyAndVisible()
        return true
    }
    
    // MARK: - Navigation Flows
    
    private func showOnboarding(with binder: OperacionesAppBinder) {
        let onboardingVC = OnboardingController()
        onboardingVC.onStart = { [weak self] in
            self?.showInitialCurrencySetup(from: onboardingVC)
        }
        
        onboardingVC.onLoadMock = { [weak self] in
            binder.cargarMock()
            // After loading mock, we should have currencies, so show main app
            self?.showMainApp(with: binder)
        }
        
        window?.rootViewController = onboardingVC
    }
    
    private func showMainApp(with binder: OperacionesAppBinder) {
        let rootController = OperacionesController()
        rootController.configure(with: binder)
        
        let navController = UINavigationController(rootViewController: rootController)
        navController.navigationBar.prefersLargeTitles = true
        
        setupMenuWiring(for: rootController, in: navController)
        
        window?.rootViewController = navController
    }
    
    private func showInitialCurrencySetup(from source: UIViewController) {
        let monedasVC = MonedasController()
        let monedasBinder = MonedasAppBinder()
        monedasVC.configure(with: monedasBinder)
        monedasVC.autoOpenEditor = true
        
        // Al terminar (crear primera moneda), reiniciamos al App Principal
        monedasVC.onBackToOperations = { [weak self] in
            let newBinder = OperacionesAppBinder()
            self?.showMainApp(with: newBinder)
        }
        
        let nav = UINavigationController(rootViewController: monedasVC)
        nav.modalPresentationStyle = .fullScreen
        source.present(nav, animated: true)
    }
    
    private func setupMenuWiring(for rootController: OperacionesController, in navController: UINavigationController) {
        rootController.onMenuOptionSelected = { [weak navController] option in
            guard let navController = navController else { return }
            
            switch option {
            case "tasas":
                let tasasVC = TasasController()
                let tasasBinder = TasasAppBinder()
                tasasVC.configure(with: tasasBinder)
                navController.pushViewController(tasasVC, animated: true)
                
            case "monedas":
                let monedasVC = MonedasController()
                let monedasBinder = MonedasAppBinder()
                monedasVC.configure(with: monedasBinder)
                
                monedasVC.onBackToOperations = { [weak navController] in
                    navController?.popToRootViewController(animated: true)
                }
                
                navController.pushViewController(monedasVC, animated: true)
                
            case "monedas_empty":
                // Este caso ya casi no se usará si el AppDelegate maneja el Onboarding,
                // Pero lo dejamos por si se llama desde el Modo Dios o similar.
                let monedasVC = MonedasController()
                let monedasBinder = MonedasAppBinder()
                monedasVC.configure(with: monedasBinder)
                monedasVC.autoOpenEditor = true
                
                monedasVC.onBackToOperations = { [weak navController] in
                    navController?.popToRootViewController(animated: true)
                }
                
                navController.pushViewController(monedasVC, animated: true)
                
            case "onboarding":
                let newBinder = OperacionesAppBinder()
                self.showOnboarding(with: newBinder)
                
            default:
                break
            }
        }
        
        rootController.navigationItem.backButtonTitle = "Operaciones"
    }
    
}
