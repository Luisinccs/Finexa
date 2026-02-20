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
    
    public func bindFields(base: DcComboBox, destino: DcComboBox, valor: DcNumberTextField) {
        var vm = viewModel
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            if let ptr = TasasViewModel_selectorBase(rawPtr) {
                base.setComboBoxViewModel(ptr)
            }
            if let ptr = TasasViewModel_selectorDestino(rawPtr) {
                destino.setComboBoxViewModel(ptr)
            }
            if let ptr = TasasViewModel_inputValor(rawPtr) {
                valor.setNumberFieldViewModel(ptr)
            }
        }
    }
    
    public func bindLabels(lblBase: UILabel, lblDestino: UILabel, lblValor: UILabel) {
        var vm = viewModel
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            if let ptr = TasasViewModel_selectorBase(rawPtr) {
                lblBase.setViewModel(ptr)
            }
            if let ptr = TasasViewModel_selectorDestino(rawPtr) {
                lblDestino.setViewModel(ptr)
            }
            if let ptr = TasasViewModel_inputValor(rawPtr) {
                lblValor.setViewModel(ptr)
            }
        }
    }
    
    public func bindCommands(bar: DcCommandBar) {
        var vm = viewModel
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            if let acceptPtr = TasasViewModel_cmdGuardarTasa(rawPtr) {
                bar.btnAccept.setViewModel(acceptPtr)
            }
            if let cancelPtr = TasasViewModel_cmdCancelar(rawPtr) {
                bar.btnCancel.setViewModel(cancelPtr)
            }
        }
    }
    
    public func bindCloseRequest(handler: @escaping () -> Void) {
        var vm = viewModel
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            let closureWrapper = Unmanaged.passRetained(ClosureWrapper(closure: handler)).toOpaque()
            
            TasasViewModel_setOnRequestClose(rawPtr, closureWrapper) { ctx in
                guard let ctx = ctx else { return }
                let wrapper = Unmanaged<ClosureWrapper>.fromOpaque(ctx).takeUnretainedValue()
                DispatchQueue.main.async {
                    wrapper.closure()
                }
            }
        }
    }
    
    public func bindDialog(to viewController: UIViewController) {
        var vm = viewModel
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let dialogPtr = TasasViewModel_dialog(rawPtr) {
                DcDialogBinder.setViewModel(dialogPtr, to: viewController)
            }
        }
    }
    
    public func unbind() {
        withUnsafePointer(to: viewModel) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            if let ptr = TasasViewModel_selectorBase(rawPtr) {
                DcControl_BindLabelText(ptr, nil, nil)
                DcComboBox_BindTextChange(ptr, nil, nil)
                DcComboBox_BindListUpdate(ptr, nil, nil)
                DcComboBox_BindSelectionChange(ptr, nil, nil)
            }
            if let ptr = TasasViewModel_selectorDestino(rawPtr) {
                DcControl_BindLabelText(ptr, nil, nil)
                DcComboBox_BindTextChange(ptr, nil, nil)
                DcComboBox_BindListUpdate(ptr, nil, nil)
                DcComboBox_BindSelectionChange(ptr, nil, nil)
            }
            if let ptr = TasasViewModel_inputValor(rawPtr) {
                DcControl_BindLabelText(ptr, nil, nil)
                DcInput_BindTextChange(ptr, nil, nil)
                DcInput_BindPlaceholderChange(ptr, nil, nil)
            }
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
