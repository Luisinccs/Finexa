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
    
    public func bind(controller: EditorOperacionController) {
        withUnsafePointer(to: viewModel) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            // Bind Concepto
            if let inputPtr = OperacionesViewModel_inputConcepto(rawPtr) {
                controller.txtConcepto.setViewModel(inputPtr)
                controller.lblConcepto.setViewModel(inputPtr)
            }
            
            // Bind Monto
            if let numberPtr = OperacionesViewModel_inputMonto(rawPtr) {
                controller.txtMonto.setNumberFieldViewModel(numberPtr)
                controller.lblMonto.setViewModel(numberPtr)
            }
            
            // Bind Moneda
            if let comboPtr = OperacionesViewModel_selectorMoneda(rawPtr) {
                controller.cmbMoneda.setComboBoxViewModel(comboPtr)
                controller.lblMoneda.setViewModel(comboPtr)
            }
            
            // Bind Monto Ref (UITextField read-only)
            if let labelPtr = OperacionesViewModel_labelMontoXds(rawPtr) {
                controller.txtMontoBase.setViewModel(labelPtr)
                // Make it read-only for user input but allow programmed updates
                controller.txtMontoBase.inputView = UIView() 
                controller.lblMontoBase.setViewModel(labelPtr)
            }
            
            // Bind Commands
            if let ptr = OperacionesViewModel_cmdAgregar(rawPtr) {
                controller.cmdAceptar.setViewModel(ptr)
            }
            if let ptr = OperacionesViewModel_cmdCancelar(rawPtr) {
                controller.cmdCancelar.setViewModel(ptr)
            }
            
            // Bind Close Request
            // Convertir el closure de Swift a un puntero opaco para C++
            let closureWrapper = Unmanaged.passRetained(ClosureWrapper(closure: { [weak controller] in
                controller?.dismiss(animated: true)
            })).toOpaque()
            
            OperacionesViewModel_setOnRequestClose(rawPtr, closureWrapper) { ctx in
                guard let ctx = ctx else { return }
                let wrapper = Unmanaged<ClosureWrapper>.fromOpaque(ctx).takeUnretainedValue()
                DispatchQueue.main.async {
                    wrapper.closure()
                }
            }
        }
    }
    
    public func unbind() {
        withUnsafePointer(to: viewModel) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            // Helpers
            let unbindControl: (UnsafeMutableRawPointer) -> Void = { ptr in
                DcControl_BindLabelText(ptr, nil, nil)
                DcControl_BindEnabledState(ptr, nil, nil)
                DcControl_BindVisibleState(ptr, nil, nil)
            }
            let unbindInput: (UnsafeMutableRawPointer) -> Void = { ptr in
                unbindControl(ptr)
                DcInput_BindTextChange(ptr, nil, nil)
                DcInput_BindPlaceholderChange(ptr, nil, nil)
            }
            let unbindNumber: (UnsafeMutableRawPointer) -> Void = { ptr in
                unbindControl(ptr)
                DcNumber_BindTextChange(ptr, nil, nil)
                DcNumber_BindValueChange(ptr, nil, nil)
            }
            let unbindCombo: (UnsafeMutableRawPointer) -> Void = { ptr in
                unbindControl(ptr)
                DcComboBox_BindTextChange(ptr, nil, nil)
                DcComboBox_BindListUpdate(ptr, nil, nil)
                DcComboBox_BindSelectionChange(ptr, nil, nil)
            }
            
            if let ptr = OperacionesViewModel_inputConcepto(rawPtr) { unbindInput(ptr) }
            if let ptr = OperacionesViewModel_inputMonto(rawPtr) { unbindNumber(ptr) }
            if let ptr = OperacionesViewModel_labelMontoXds(rawPtr) { unbindInput(ptr) }
            if let ptr = OperacionesViewModel_selectorMoneda(rawPtr) { unbindCombo(ptr) }
            
            if let ptr = OperacionesViewModel_cmdAgregar(rawPtr) { unbindControl(ptr) }
            if let ptr = OperacionesViewModel_cmdCancelar(rawPtr) { unbindControl(ptr) }
            if let ptr = OperacionesViewModel_cmdEliminar(rawPtr) { unbindControl(ptr) }
        }
    }
}

// Helper para pasar closures a C++
private class ClosureWrapper {
    let closure: () -> Void
    init(closure: @escaping () -> Void) {
        self.closure = closure
    }
}

