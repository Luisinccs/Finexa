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
    func bindFields(concepto: UITextField, monto: DcNumberTextField, moneda: DcComboBox, montoRef: UITextField)
    
    /// Vincula las etiquetas de los campos del editor con el ViewModel.
    /// - Parameters:
    ///   - lblConcepto: Etiqueta para el concepto.
    ///   - lblMonto: Campo numérico para el monto original.
    ///   - lblMoneda: Selector de moneda.
    ///   - lblMontoBase: Label para el monto referencial calculado.
    func bindLabels(lblConcepto: UILabel, lblMonto: UILabel, lblMoneda: UILabel, lblMontoBase: UILabel)
    
    /// Vincula los comandos del editor con el ViewModel.
    /// - Parameters:
    ///   - cmdAceptar: Botón de aceptar.
    ///   - cmdCancelar: Botón de cancelar.
    func bindCommands(cmdAceptar: UIButton, cmdCancelar: UIButton)
    
    /// Vincula la solicitud de cierre del ViewModel (al completar la operación).
    /// - Parameter onClose: Closure a ejecutar cuando se solicite el cierre.
    func bindCloseRequest(onClose: @escaping () -> Void)
        
}
