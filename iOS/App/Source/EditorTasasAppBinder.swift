import UIKit
import DcViewModels
import DcViewsIos
import FinexaViewsIos
import DcBindingsIos

public class EditorTasasAppBinder: EditorTasaBinder {
    
    private var viewModel: Finexa.ViewModels.TasasViewModelPtr
    
    init(viewModel: Finexa.ViewModels.TasasViewModelPtr) {
        self.viewModel = viewModel
    }
    
    public func bind(controller: EditorTasaController) {
        var vm = viewModel
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            // Bind Fields & Labels
            if let ptr = TasasViewModel_selectorBase(rawPtr) {
                controller.cmbBase.setComboBoxViewModel(ptr)
                controller.lblBase.setViewModel(ptr)
            }
            if let ptr = TasasViewModel_selectorDestino(rawPtr) {
                controller.cmbDestino.setComboBoxViewModel(ptr)
                controller.lblDestino.setViewModel(ptr)
            }
            if let ptr = TasasViewModel_inputValor(rawPtr) {
                controller.txtValor.setNumberFieldViewModel(ptr)
                controller.lblValor.setViewModel(ptr)
            }
            
            // Bind Commands
            if let acceptPtr = TasasViewModel_cmdGuardarTasa(rawPtr) {
                controller.commandBar.btnAccept.setViewModel(acceptPtr)
            }
            if let cancelPtr = TasasViewModel_cmdCancelar(rawPtr) {
                controller.commandBar.btnCancel.setViewModel(cancelPtr)
            }
            
            // Bind Close Request
            let closureWrapper = Unmanaged.passRetained(ClosureWrapper(closure: { [weak controller] in
                controller?.dismiss(animated: true)
            })).toOpaque()
            
            TasasViewModel_setOnRequestClose(rawPtr, closureWrapper) { ctx in
                guard let ctx = ctx else { return }
                let wrapper = Unmanaged<ClosureWrapper>.fromOpaque(ctx).takeUnretainedValue()
                DispatchQueue.main.async {
                    wrapper.closure()
                }
            }
            
            // Bind Dialog
            if let dialogPtr = TasasViewModel_dialog(rawPtr) {
                DcDialogBinder.setViewModel(dialogPtr, to: controller)
            }
        }
    }
    
    public func unbind() {
        withUnsafePointer(to: viewModel) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            let unbindControl: (UnsafeMutableRawPointer) -> Void = { ptr in
                DcControl_BindLabelText(ptr, nil, nil)
                DcControl_BindEnabledState(ptr, nil, nil)
                DcControl_BindVisibleState(ptr, nil, nil)
            }
            let _: (UnsafeMutableRawPointer) -> Void = { ptr in
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
            
            if let ptr = TasasViewModel_selectorBase(rawPtr) { unbindCombo(ptr) }
            if let ptr = TasasViewModel_selectorDestino(rawPtr) { unbindCombo(ptr) }
            if let ptr = TasasViewModel_inputValor(rawPtr) { unbindNumber(ptr) }
            
            // Commands
            if let ptr = TasasViewModel_cmdGuardarTasa(rawPtr) { unbindControl(ptr) }
            if let ptr = TasasViewModel_cmdCancelar(rawPtr) { unbindControl(ptr) }
        }
    }
}

// Helpers
private class ClosureWrapper {
    let closure: () -> Void
    init(closure: @escaping () -> Void) { self.closure = closure }
}

@_silgen_name("TasasViewModel_setOnRequestClose")
func TasasViewModel_setOnRequestClose(_ vmPtr: UnsafeMutableRawPointer, _ closureCtx: UnsafeMutableRawPointer, _ callback: @convention(c) (UnsafeMutableRawPointer?) -> Void)

@_silgen_name("TasasViewModel_dialog")
func TasasViewModel_dialog(_ vmPtr: UnsafeMutableRawPointer) -> UnsafeMutableRawPointer?

@_silgen_name("TasasViewModel_cmdCancelar")
func TasasViewModel_cmdCancelar(_ vmPtr: UnsafeMutableRawPointer) -> UnsafeMutableRawPointer?
