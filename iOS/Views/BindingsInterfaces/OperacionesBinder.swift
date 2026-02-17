import UIKit
import DcViewsIos

/// Protocolo que define la interfaz para el binding del controlador de Operaciones (Grilla y Totales).
public protocol OperacionesBinder: AnyObject {
    
    /// Callback disparado cuando hay cambios en los datos que requieren actualizar la grilla.
    var onDataChanged: (() -> Void)? { get set }
    
    /// Callback disparado cuando cambia el texto del total.
    var onTotalChanged: ((String) -> Void)? { get set }
    
    /// Título del total acumulado.
    var totalAmountTitle: String { get }
    
    /// Conecta el ComboBox de moneda referencial con el ViewModel.
    func bindRefCurrencySelector(_ combo: DcComboBox)
    
    /// Retorna el número de operaciones.
    func numberOfRows() -> Int
    
    /// Obtiene los datos para una celda específica.
    /// Retorna (concepto, monto, moneda, montoRef).
    func cellData(for row: Int) -> (concepto: String, monto: String, moneda: String, montoXds: String)
    
    /// Selecciona una fila para editar.
    func selectRow(at row: Int)
    
    /// Prepara la vista para agregar una nueva operación.
    func requestAdd()
    
    /// Elimina una operación específica.
    func deleteRow(at row: Int)
    
    /// Retorna el binder para el formulario de edición.
    func getEditorBinder() -> EditorOperacionBinder?
}

