import UIKit
import DcViewModels
import DcViewsIos
import FinexaViewsIos

class MonedasAppBinder: MonedasBinder {
    
    private var viewModelPtr: Finexa.ViewModels.MonedasViewModelPtr

    var onDataChanged: (() -> Void)?
    
    init() {
        self.viewModelPtr = Finexa.ViewModels.MonedasViewModel.create()
        setupBinding()
    }
    
    private func setupBinding() {
        let vm = viewModelPtr
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let gridPtr = MonedasViewModel_monedasGridViewModel(rawPtr) {
                // Bind Data Changed
                DcGrid_BindDataChanged(gridPtr, Unmanaged.passUnretained(self).toOpaque()) { ctx in
                    guard let ctx = ctx else { return }
                    let binder = Unmanaged<MonedasAppBinder>.fromOpaque(ctx).takeUnretainedValue()
                    
                    DispatchQueue.main.async {
                        binder.onDataChanged?()
                    }
                }
            }
        }
    }
    
    func numberOfRows() -> Int {
        var count: Int32 = 0
        let vm = viewModelPtr
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let gridPtr = MonedasViewModel_monedasGridViewModel(rawPtr) {
                 count = DcGrid_GetRowCount(gridPtr)
                 // Hide ghost row on iOS
                 if count > 0 && DcGrid_IsRowGhost(gridPtr, count - 1) {
                     count -= 1
                 }
            }
        }
        return Int(count)
    }
    
    func cellData(for row: Int) -> (nombre: String, simbolo: String, siglas: String, isDeletable: Bool) {
        let vm = viewModelPtr
        var nombre = "", simbolo = "", siglas = "", isDeletable = true
        
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let gridPtr = MonedasViewModel_monedasGridViewModel(rawPtr) {
                nombre = getCellText(gridPtr, row, 0)
                siglas = getCellText(gridPtr, row, 1)
                simbolo = getCellText(gridPtr, row, 2)
                isDeletable = getCellText(gridPtr, row, 3) == "true"
            }
        }
        return (nombre, simbolo, siglas, isDeletable)
    }
    
    private func getCellText(_ gridPtr: UnsafeMutableRawPointer, _ row: Int, _ col: Int) -> String {
        let bufferSize = 256
        var buffer = [CChar](repeating: 0, count: bufferSize)
        DcGrid_GetRowCell(gridPtr, Int32(row), Int32(col), &buffer, Int32(bufferSize))
        return String(cString: buffer)
    }
    
    func selectRow(at row: Int) {
        let vm = viewModelPtr
        withUnsafePointer(to: vm) { vmPtr in
             let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
             if let gridPtr = MonedasViewModel_monedasGridViewModel(rawPtr) {
                 DcGrid_SelectRow(gridPtr, Int32(row))
                 // Trigger activation logic (populates editor fields in VM)
                 DcGrid_ActivateRow(gridPtr)
             }
         }
    }
    
    func requestAdd() {
        let vm = viewModelPtr
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let gridPtr = MonedasViewModel_monedasGridViewModel(rawPtr) {
                DcGrid_RequestAdd(gridPtr)
            }
        }
    }
    
    func deleteRow(at row: Int) {
        selectRow(at: row)
        let vm = viewModelPtr
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let cmdPtr = MonedasViewModel_eliminarViewModel(rawPtr) {
                DcCommand_Execute(cmdPtr)
            }
        }
    }
    
    func getEditorBinder() -> EditorMonedaBinder? {
        return EditorMonedaAppBinder(viewModel: viewModelPtr)
    }
}
