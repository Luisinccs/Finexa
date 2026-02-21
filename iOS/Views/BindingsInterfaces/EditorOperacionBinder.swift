import UIKit
import DcViewsIos

/// Protocolo que define la interfaz para el binding del editor de Operaciones.
public protocol EditorOperacionBinder: AnyObject {
    
    /// Vincula los controles del editor con el ViewModel.
    /// - Parameter controller: El controlador que contiene los elementos de UI a vincular.
    func bind(controller: EditorOperacionController)
    
    /// Desvincula todos los controles para evitar crashes por punteros colgantes
    /// a vistas que han sido destruidas.
    func unbind()
}
