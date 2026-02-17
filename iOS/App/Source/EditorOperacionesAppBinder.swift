import UIKit
import DcViewModels
import FinexaViewsIos
import DcBindingsIos
import DcViewsIos

public class EditorOperacionesAppBinder: EditorOperacionBinder {
    
    // Almacenar el shared_ptr directamente para mantener vivo el ViewModel C++
    private var viewModel: Finexa.ViewModels.OperacionesViewModelPtr
    
    public init(viewModel: Finexa.ViewModels.OperacionesViewModelPtr) {
        self.viewModel = viewModel
    }
    
    public func bind(concepto: UITextField, monto: DcNumberTextField, moneda: DcComboBox, montoRef: UILabel) {
        withUnsafePointer(to: viewModel) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            // Bind Concepto
            if let inputPtr = OperacionesViewModel_inputConcepto(rawPtr) {
                concepto.setViewModel(inputPtr)
            }
            
            // Bind Monto
            if let numberPtr = OperacionesViewModel_inputMonto(rawPtr) {
                monto.setNumberFieldViewModel(numberPtr)
            }
            
            // Bind Moneda
            if let comboPtr = OperacionesViewModel_selectorMoneda(rawPtr) {
                moneda.setComboBoxViewModel(comboPtr)
            }
            
            // Bind Monto Ref
            if let labelPtr = OperacionesViewModel_labelMontoXds(rawPtr) {
                montoRef.setViewModel(labelPtr)
            }
        }
    }
    
    public func save() {
        withUnsafePointer(to: viewModel) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let cmdPtr = OperacionesViewModel_cmdAgregar(rawPtr) {
                DcCommand_Execute(cmdPtr)
            }
        }
    }
    
    public func cancel() {
        // ViewModel handles clear on save.
        // Just close the view.
    }
}

