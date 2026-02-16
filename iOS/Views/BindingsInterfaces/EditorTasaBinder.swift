import UIKit
import DcViewsIos

/// Protocolo para bindear los controles del editor de Tasas a la lógica de negocio.
public protocol EditorTasaBinder {
    
    /// Vincula los controles de UI con el ViewModel subyacente.
    /// - Parameters:
    ///   - base: Selector (DcComboBox) para la moneda base.
    ///   - destino: Selector (DcComboBox) para la moneda destino.
    ///   - valor: Campo numérico (DcNumberTextField) para el valor de la tasa.
    func bind(base: DcComboBox, destino: DcComboBox, valor: DcNumberTextField)
    
    /// Ejecuta la acción de guardar/aceptar.
    func save()
    
    /// Ejecuta la acción de cancelar.
    func cancel()
}
