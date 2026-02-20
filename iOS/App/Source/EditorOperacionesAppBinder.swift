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
    
    public func bindFields(concepto: UITextField, monto: DcNumberTextField, moneda: DcComboBox, montoRef: UITextField) {
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
            
            // Bind Monto Ref (UITextField read-only)
            if let labelPtr = OperacionesViewModel_labelMontoXds(rawPtr) {
                montoRef.setViewModel(labelPtr)
                // Make it read-only for user input but allow programmed updates
                montoRef.inputView = UIView() 
            }
        }
    }
    
    public func bindLabels(lblConcepto: UILabel, lblMonto: UILabel, lblMoneda: UILabel, lblMontoBase: UILabel) {
        withUnsafePointer(to: viewModel) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            if let ptr = OperacionesViewModel_inputConcepto(rawPtr) {
                lblConcepto.setViewModel(ptr)
            }
            if let ptr = OperacionesViewModel_inputMonto(rawPtr) {
                lblMonto.setViewModel(ptr)
            }
            if let ptr = OperacionesViewModel_selectorMoneda(rawPtr) {
                lblMoneda.setViewModel(ptr)
            }
            if let ptr = OperacionesViewModel_labelMontoXds(rawPtr) {
                lblMontoBase.setViewModel(ptr)
            }
        }
    }
    
    public func bindCommands(cmdAceptar: UIButton, cmdCancelar: UIButton) {
        withUnsafePointer(to: viewModel) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            if let ptr = OperacionesViewModel_cmdAgregar(rawPtr) {
                cmdAceptar.setViewModel(ptr)
            }
            
            if let ptr = OperacionesViewModel_cmdCancelar(rawPtr) {
                cmdCancelar.setViewModel(ptr)
            }
        }
    }
    
    public func bindCloseRequest(onClose: @escaping () -> Void) {
        withUnsafePointer(to: viewModel) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            // Convertir el closure de Swift a un puntero opaco para C++
            // Necesitamos mantener una referencia al closure para que no se libere
            let closureWrapper = Unmanaged.passRetained(ClosureWrapper(closure: onClose)).toOpaque()
            
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
            
            // Clear bindings for Concepto
            if let ptr = OperacionesViewModel_inputConcepto(rawPtr) {
                DcControl_BindLabelText(ptr, nil, nil)
                DcInput_BindTextChange(ptr, nil, nil)
                DcInput_BindPlaceholderChange(ptr, nil, nil)
            }
            
            // Clear bindings for Monto
            if let ptr = OperacionesViewModel_inputMonto(rawPtr) {
                DcControl_BindLabelText(ptr, nil, nil)
                DcInput_BindTextChange(ptr, nil, nil)
                DcInput_BindPlaceholderChange(ptr, nil, nil)
            }
            
            // Clear bindings for Moneda Selector
            if let ptr = OperacionesViewModel_selectorMoneda(rawPtr) {
                DcControl_BindLabelText(ptr, nil, nil)
                DcComboBox_BindTextChange(ptr, nil, nil)
                DcComboBox_BindListUpdate(ptr, nil, nil)
                DcComboBox_BindSelectionChange(ptr, nil, nil)
            }
            
            // Clear bindings for Monto Ref
            if let ptr = OperacionesViewModel_labelMontoXds(rawPtr) {
                DcControl_BindLabelText(ptr, nil, nil)
                DcInput_BindTextChange(ptr, nil, nil)
                DcInput_BindPlaceholderChange(ptr, nil, nil)
            }
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

