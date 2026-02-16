import UIKit
import DcViewModels
import FinexaViewsIos
import DcBindingsIos
import DcViewsIos

public class EditorOperacionesAppBinder: EditorOperacionBinder {
    
    private var viewModelPtr: UnsafeMutableRawPointer? // OperacionesViewModelPtr as opaque
    
    public init(viewModel: Finexa.ViewModels.OperacionesViewModelPtr) {
        // We need to keep a reference or just use it. 
        // In the C++ model, the ViewModel is shared.
        // Convert to opaque for storage/usage with C-Bridge
        withUnsafePointer(to: viewModel) { vmPtr in
            self.viewModelPtr = UnsafeMutableRawPointer(mutating: vmPtr)
        }
    }
    
    public func bind(concepto: UITextField, monto: DcNumberTextField, moneda: DcComboBox, montoRef: UILabel) {
        guard let vmPtr = viewModelPtr else { return }
        
        // Bind Concepto
        if let inputPtr = OperacionesViewModel_inputConcepto(vmPtr) {
            concepto.setViewModel(inputPtr)
        }
        
        // Bind Monto
        if let numberPtr = OperacionesViewModel_inputMonto(vmPtr) {
            monto.bindViewModel(numberPtr)
        }
        
        // Bind Moneda
        if let comboPtr = OperacionesViewModel_selectorMoneda(vmPtr) {
            moneda.bindViewModel(comboPtr)
        }
        
        // Bind Monto Ref
        if let labelPtr = OperacionesViewModel_labelMontoXds(vmPtr) {
            montoRef.setViewModel(labelPtr)
        }
    }
    
    public func save() {
        guard let vmPtr = viewModelPtr else { return }
        if let cmdPtr = OperacionesViewModel_cmdAgregar(vmPtr) {
            DcCommand_Execute(cmdPtr)
        }
    }
    
    public func cancel() {
        // Just dismiss or clear?
        // ViewModel handles clear on save, but maybe we want to revert selection?
        // OperacionesViewModel doesn't explicitly have 'cancel' logic other than clearing.
        // We can just close the view.
    }
}
