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
            // 2. Configurar UI con Navigation Controller
            let rootController = OperacionesController()
            let binder = OperacionesAppBinder()
            rootController.configure(with: binder)
            
            let navController = UINavigationController(rootViewController: rootController)
            
            // Estilo iOS moderno (Títulos grandes)
            navController.navigationBar.prefersLargeTitles = true
            
            // 3. Wiring del menú hamburguesa
            rootController.onMenuOptionSelected = { [weak navController] option in
                switch option {
                case "tasas":
                    let tasasVC = TasasController()
                    let tasasBinder = TasasAppBinder()
                    tasasVC.configure(with: tasasBinder)
                    navController?.pushViewController(tasasVC, animated: true)
                    
                case "monedas":
                    let monedasVC = MonedasController()
                    let monedasBinder = MonedasAppBinder()
                    monedasVC.configure(with: monedasBinder)
                    
                    monedasVC.onBackToOperations = { [weak navController] in
                        navController?.popToRootViewController(animated: true)
                    }
                    
                    navController?.pushViewController(monedasVC, animated: true)
                    
                case "monedas_empty":
                    let monedasVC = MonedasController()
                    let monedasBinder = MonedasAppBinder()
                    monedasVC.configure(with: monedasBinder)
                    monedasVC.autoOpenEditor = true // Open editor immediately
                    
                    monedasVC.onBackToOperations = { [weak navController] in
                        navController?.popToRootViewController(animated: true)
                    }
                    
                    navController?.pushViewController(monedasVC, animated: true)
                    
                default:
                    break
                }
            }
            
            // Back button title for pushed controllers
            rootController.navigationItem.backButtonTitle = "Operaciones"
            
            window?.rootViewController = navController
            window?.makeKeyAndVisible()
            
            return true
        }
    
}
