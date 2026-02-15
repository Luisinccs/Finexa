//  2026/02/15

import UIKit
import FinexaViewsIos

@main
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?

        func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
            // 1. Configuración de Ventana
            window = UIWindow(frame: UIScreen.main.bounds)
                            
            // 6. Configurar UI con Navigation Controller
            let rootController = MonedasController()
            let binder = MonedasControllerMockBinder()
            rootController.configure(with: binder)
                        
            let navController = UINavigationController(rootViewController: rootController)
            
            // Estilo iOS moderno (Títulos grandes)
            navController.navigationBar.prefersLargeTitles = true
            
            window?.rootViewController = navController
            window?.makeKeyAndVisible()
            
            return true
        }

}
