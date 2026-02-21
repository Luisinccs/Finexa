import UIKit
import DcViewModels
import DcViewsIos
import FinexaViewsIos
import DcBindingsIos

public class TasasAppBinder: TasasBinder {
    
    private var viewModelPtr: Finexa.ViewModels.TasasViewModelPtr
    
    public var onDataChanged: (() -> Void)?
    
    public init() {
        self.viewModelPtr = Finexa.ViewModels.TasasViewModel.create()
        setupBinding()
    }
    
    private func setupBinding() {
        let vm = viewModelPtr
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let gridPtr = TasasViewModel_gridTasas(rawPtr) {
                // Bind Data Changed
                DcGrid_BindDataChanged(gridPtr, Unmanaged.passUnretained(self).toOpaque()) { ctx in
                    guard let ctx = ctx else { return }
                    let binder = Unmanaged<TasasAppBinder>.fromOpaque(ctx).takeUnretainedValue()
                    
                    DispatchQueue.main.async {
                        binder.onDataChanged?()
                    }
                }
            }
        }
    }
    
    public func numberOfRows() -> Int {
        var count: Int32 = 0
        let vm = viewModelPtr
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let gridPtr = TasasViewModel_gridTasas(rawPtr) {
                 count = DcGrid_GetRowCount(gridPtr)
                 // Hide ghost row on iOS
                 if count > 0 && DcGrid_IsRowGhost(gridPtr, count - 1) {
                     count -= 1
                 }
            }
        }
        return Int(count)
    }
    
    public func cellData(for row: Int) -> (paridad: String, valor: String) {
        let vm = viewModelPtr
        var paridad = "", valor = ""
        
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let gridPtr = TasasViewModel_gridTasas(rawPtr) {
                paridad = getCellText(gridPtr, row, 0)
                valor = getCellText(gridPtr, row, 1)
            }
        }
        return (paridad, valor)
    }
    
    private func getCellText(_ gridPtr: UnsafeMutableRawPointer, _ row: Int, _ col: Int) -> String {
        let bufferSize = 256
        var buffer = [CChar](repeating: 0, count: bufferSize)
        DcGrid_GetRowCell(gridPtr, Int32(row), Int32(col), &buffer, Int32(bufferSize))
        return String(cString: buffer)
    }
    
    public func selectRow(at row: Int) {
        let vm = viewModelPtr
        withUnsafePointer(to: vm) { vmPtr in
             let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
             if let gridPtr = TasasViewModel_gridTasas(rawPtr) {
                 DcGrid_SelectRow(gridPtr, Int32(row))
                 DcGrid_ActivateRow(gridPtr) // Importante para cargar los datos en el editor
             }
         }
    }
    
    public func requestAdd() {
        let vm = viewModelPtr
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let gridPtr = TasasViewModel_gridTasas(rawPtr) {
                DcGrid_RequestAdd(gridPtr)
            }
        }
    }
    
    public func deleteRow(at row: Int) {
        let vm = viewModelPtr
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let cmdPtr = TasasViewModel_cmdEliminar(rawPtr) {
                DcCommand_Execute(cmdPtr)
            }
        }
    }
    
    public func getEditorBinder() -> EditorTasaBinder? {
        return EditorTasasAppBinder(viewModel: viewModelPtr)
    }
}

// C-Bridge imports
@_silgen_name("TasasViewModel_cmdEliminar")
func TasasViewModel_cmdEliminar(_ vmPtr: UnsafeMutableRawPointer) -> UnsafeMutableRawPointer?
