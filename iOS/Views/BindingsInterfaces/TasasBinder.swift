import UIKit

/// Protocolo que define la interfaz para el binding del controlador de Tasas.
/// Abstrae la lógica de datos y acciones del ViewModel subyacente.
public protocol TasasBinder: AnyObject {
    
    /// Callback disparado cuando hay cambios en los datos que requieren actualizar la vista.
    var onDataChanged: (() -> Void)? { get set }
    
    /// Retorna el número de tasas a mostrar.
    func numberOfRows() -> Int
    
    /// Obtiene los datos para una celda específica.
    /// Retorna una tupla con (paridad, valor).
    func cellData(for row: Int) -> (paridad: String, valor: String)
    
    /// Selecciona una fila para editar.
    func selectRow(at row: Int)
    
    /// Prepara la vista para agregar una nueva tasa.
    func requestAdd()
    
    /// Elimina una tasa específica.
    func deleteRow(at row: Int)
    
    /// Retorna el binder para el formulario de edición de tasas.
    func getEditorBinder() -> EditorTasaBinder?
}
