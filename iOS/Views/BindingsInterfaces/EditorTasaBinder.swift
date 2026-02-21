import UIKit
import DcViewsIos

/// Protocolo para bindear los controles del editor de Tasas a la lógica de negocio.
public protocol EditorTasaBinder {
    func bind(controller: EditorTasaController)
    func unbind()
}
