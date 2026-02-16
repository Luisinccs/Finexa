import UIKit
import DcViewsIos

/// Protocolo que define la interfaz para el binding del editor de Operaciones.
public protocol EditorOperacionBinder: AnyObject {
    
    /// Vincula los controles del editor con el ViewModel.
    /// - Parameters:
    ///   - concepto: Campo de texto para el concepto.
    ///   - monto: Campo numérico para el monto original.
    ///   - moneda: Selector de moneda.
    ///   - montoRef: Label para el monto referencial calculado.
    func bind(concepto: UITextField, monto: DcNumberTextField, moneda: DcComboBox, montoRef: UILabel)
    
    /// Ejecuta la acción de guardar/actualizar.
    func save()
    
    /// Cancela la edición.
    func cancel()
}
