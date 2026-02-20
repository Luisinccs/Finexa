import UIKit
import DcViewsIos

/// Protocolo para bindear los controles del editor de Tasas a la lógica de negocio.
public protocol EditorTasaBinder {
    func bindFields(base: DcComboBox, destino: DcComboBox, valor: DcNumberTextField)
    func bindLabels(lblBase: UILabel, lblDestino: UILabel, lblValor: UILabel)
    func bindCommands(bar: DcCommandBar)
    func bindCloseRequest(handler: @escaping () -> Void)
    func bindDialog(to viewController: UIViewController)
    func unbind()
}
