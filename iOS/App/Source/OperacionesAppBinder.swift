import UIKit
import DcViewModels
import FinexaViewsIos
import DcBindingsIos
import DcViewsIos

public class OperacionesAppBinder: OperacionesBinder {
    
    private var viewModelPtr: Finexa.ViewModels.OperacionesViewModelPtr
    
    public var onDataChanged: (() -> Void)?
    public var onTotalChanged: ((String) -> Void)?
    
    // Cache for current total value
    private var _totalTitle: String = "Total: ..."
    
    public var totalAmountTitle: String { return _totalTitle }
    
    public init() {
        self.viewModelPtr = Finexa.ViewModels.OperacionesViewModel.create()
        setupBinding()
    }
    
    private func setupBinding() {
        let vm = viewModelPtr
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            // 1. Bind Grid Data Changed
            if let gridPtr = OperacionesViewModel_gridOperaciones(rawPtr) {
                DcGrid_BindDataChanged(gridPtr, Unmanaged.passUnretained(self).toOpaque()) { ctx in
                    guard let ctx = ctx else { return }
                    let binder = Unmanaged<OperacionesAppBinder>.fromOpaque(ctx).takeUnretainedValue()
                    DispatchQueue.main.async {
                        binder.onDataChanged?()
                    }
                }
            }
            
            // 2. Bind Total Label
            if let totalLabelPtr = OperacionesViewModel_labelTotal(rawPtr) {
                 DcControl_BindLabelText(totalLabelPtr, Unmanaged.passUnretained(self).toOpaque()) { ctx, str in
                     guard let ctx = ctx, let s = str else { return }
                     let text = String(cString: s)
                     let binder = Unmanaged<OperacionesAppBinder>.fromOpaque(ctx).takeUnretainedValue()
                     DispatchQueue.main.async {
                         binder._totalTitle = text
                         binder.onTotalChanged?(text)
                     }
                 }
            }
        }
    }
    
    // MARK: - Bind Reference Currency ComboBox
    
    public func bindRefCurrencySelector(_ combo: DcComboBox) {
        let vm = viewModelPtr
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let comboPtr = OperacionesViewModel_selectorMonedaRef(rawPtr) {
                combo.setComboBoxViewModel(comboPtr)
            }
        }
    }
    
    // MARK: - Data Access
    
    public func numberOfRows() -> Int {
        var count: Int32 = 0
        let vm = viewModelPtr
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let gridPtr = OperacionesViewModel_gridOperaciones(rawPtr) {
                 count = DcGrid_GetRowCount(gridPtr)
                 // Hide ghost row on iOS if present
                 if count > 0 && DcGrid_IsRowGhost(gridPtr, count - 1) {
                     count -= 1
                 }
            }
        }
        return Int(count)
    }
    
    public func cellData(for row: Int) -> (concepto: String, monto: String, moneda: String, montoXds: String) {
        let vm = viewModelPtr
        var concepto = "", monto = "", moneda = "", montoXds = ""
        
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let gridPtr = OperacionesViewModel_gridOperaciones(rawPtr) {
                concepto = getCellText(gridPtr, row, 0)
                monto = getCellText(gridPtr, row, 1)
                moneda = getCellText(gridPtr, row, 2)
                montoXds = getCellText(gridPtr, row, 3)
            }
        }
        return (concepto, monto, moneda, montoXds)
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
             OperacionesViewModel_cargarOperacion(rawPtr, Int32(row))
         }
    }
    
    public func requestAdd() {
        let vm = viewModelPtr
        withUnsafePointer(to: vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            OperacionesViewModel_limpiarEditor(rawPtr)
        }
    }
    
    public func deleteRow(at row: Int) {
        let vm = viewModelPtr
        withUnsafePointer(to: vm) { vmPtr in
             let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
             OperacionesViewModel_cargarOperacion(rawPtr, Int32(row))
             
             if let cmdPtr = OperacionesViewModel_cmdEliminar(rawPtr) {
                 DcCommand_Execute(cmdPtr)
             }
         }
    }
    
    public func getEditorBinder() -> EditorOperacionBinder? {
        return EditorOperacionesAppBinder(viewModel: viewModelPtr)
    }
}
