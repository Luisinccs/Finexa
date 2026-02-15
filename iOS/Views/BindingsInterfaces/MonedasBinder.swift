import UIKit

/// Protocol that abstracts the data source and actions for the Monedas list.
public protocol MonedasBinder: AnyObject {
    /// Callback triggered when data changes and the UI needs to refresh.
    var onDataChanged: (() -> Void)? { get set }
    
    /// Returns the number of items to display.
    func numberOfRows() -> Int
    
    /// Returns the text for a specific row.
    func cellData(for row: Int) -> (nombre: String, simbolo: String, siglas: String)
    
    /// Selects a row (e.g., to prepare for editing).
    func selectRow(at row: Int)
    
    /// Prepares for adding a new item.
    func requestAdd()
    
    /// Deletes a specific row.
    func deleteRow(at row: Int)
    
    /// Returns a binder for the editor of the currently selected row or a new one.
    func getEditorBinder() -> EditorMonedaBinder?
}
