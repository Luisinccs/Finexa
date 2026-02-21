import UIKit
import DcViewModels
import DcViewsIos
import FinexaViewsIos
import DcBindingsIos

// MARK: - Concrete Implementation

/// Concrete implementation of the binder using DcViewModels.
class EditorMonedaAppBinder: EditorMonedaBinder {
    
    private var viewModel: Finexa.ViewModels.MonedasViewModelPtr
    
    init(viewModel: Finexa.ViewModels.MonedasViewModelPtr) {
        self.viewModel = viewModel
    }
    
    func bind(controller: EditorMonedaController) {
        var vm = viewModel
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)

            let context = Unmanaged.passUnretained(controller).toOpaque()

            // Bind Fields & Labels
            if let ptr = MonedasViewModel_nombreViewModel(rawPtr) {
                controller.txtNombre.setViewModel(ptr)
                controller.lblNombre.setViewModel(ptr)
                DcInput_BindErrorTextChange(ptr, context) { ctx, str in
                    guard let ctx = ctx else { return }
                    let ctrl = Unmanaged<EditorMonedaController>.fromOpaque(ctx).takeUnretainedValue()
                    let text = str != nil ? String(cString: str!) : nil
                    DispatchQueue.main.async { ctrl.txtNombre.errorText = text?.isEmpty == true ? nil : text }
                }
            }
            
            if let ptr = MonedasViewModel_simboloViewModel(rawPtr) {
                controller.txtSimbolo.setViewModel(ptr)
                controller.lblSimbolo.setViewModel(ptr)
                DcInput_BindErrorTextChange(ptr, context) { ctx, str in
                    guard let ctx = ctx else { return }
                    let ctrl = Unmanaged<EditorMonedaController>.fromOpaque(ctx).takeUnretainedValue()
                    let text = str != nil ? String(cString: str!) : nil
                    DispatchQueue.main.async { ctrl.txtSimbolo.errorText = text?.isEmpty == true ? nil : text }
                }
            }
            
            if let ptr = MonedasViewModel_siglasViewModel(rawPtr) {
                controller.txtSiglas.setViewModel(ptr)
                controller.lblSiglas.setViewModel(ptr)
                DcInput_BindErrorTextChange(ptr, context) { ctx, str in
                    guard let ctx = ctx else { return }
                    let ctrl = Unmanaged<EditorMonedaController>.fromOpaque(ctx).takeUnretainedValue()
                    let text = str != nil ? String(cString: str!) : nil
                    DispatchQueue.main.async { ctrl.txtSiglas.errorText = text?.isEmpty == true ? nil : text }
                }
            }
            
            // Bind Commands
            let acceptPtr = MonedasViewModel_aceptarViewModel(rawPtr)
            let cancelPtr = MonedasViewModel_cancelarViewModel(rawPtr)
            
            if let accept = acceptPtr, let cancel = cancelPtr {
                controller.commandBar.btnAccept.setViewModel(accept)
                controller.commandBar.btnCancel.setViewModel(cancel)
            }
            
            // Bind Close Request
            let closureWrapper = Unmanaged.passRetained(ClosureWrapper(closure: { [weak controller] in
                controller?.dismiss(animated: true)
            })).toOpaque()
            
            MonedasViewModel_setOnRequestClose(rawPtr, closureWrapper) { ctx in
                guard let ctx = ctx else { return }
                let wrapper = Unmanaged<ClosureWrapper>.fromOpaque(ctx).takeUnretainedValue()
                DispatchQueue.main.async {
                    wrapper.closure()
                }
            }
            
            // Bind Dialog
            if let dialogPtr = MonedasViewModel_dialogViewModel(rawPtr) {
                print("[EditorMonedaBinder] Found dialogViewModel pointer, calling setViewModel")
                DcDialogBinder.setViewModel(dialogPtr, to: controller)
            } else {
                print("[EditorMonedaBinder] Error: MonedasViewModel_dialogViewModel returned nil")
            }
        }
    }
    
    func unbind() {
        var vm = viewModel
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            let unbindControl: (UnsafeMutableRawPointer) -> Void = { ptr in
                DcControl_BindLabelText(ptr, nil, nil)
                DcControl_BindEnabledState(ptr, nil, nil)
                DcControl_BindVisibleState(ptr, nil, nil)
            }
            let unbindInput: (UnsafeMutableRawPointer) -> Void = { ptr in
                unbindControl(ptr)
                DcInput_BindTextChange(ptr, nil, nil)
                DcInput_BindPlaceholderChange(ptr, nil, nil)
                DcInput_BindErrorTextChange(ptr, nil, nil)
            }
            
            if let ptr = MonedasViewModel_nombreViewModel(rawPtr) { unbindInput(ptr) }
            if let ptr = MonedasViewModel_simboloViewModel(rawPtr) { unbindInput(ptr) }
            if let ptr = MonedasViewModel_siglasViewModel(rawPtr) { unbindInput(ptr) }
            
            if let ptr = MonedasViewModel_aceptarViewModel(rawPtr) { unbindControl(ptr) }
            if let ptr = MonedasViewModel_cancelarViewModel(rawPtr) { unbindControl(ptr) }
        }
    }
}

// Helpers
private class ClosureWrapper {
    let closure: () -> Void
    init(closure: @escaping () -> Void) { self.closure = closure }
}

// MARK: - Extensions for Custom Controls
// HaloTextField extension removed.

// Ensure C function is available
@_silgen_name("MonedasViewModel_setOnRequestClose")
func MonedasViewModel_setOnRequestClose(_ vmPtr: UnsafeMutableRawPointer, _ closureCtx: UnsafeMutableRawPointer, _ callback: @convention(c) (UnsafeMutableRawPointer?) -> Void)

@_silgen_name("MonedasViewModel_dialogViewModel")
func MonedasViewModel_dialogViewModel(_ vmPtr: UnsafeMutableRawPointer) -> UnsafeMutableRawPointer?

@_silgen_name("DcInput_BindErrorTextChange")
func DcInput_BindErrorTextChange(_ ptr: UnsafeMutableRawPointer, _ ctx: UnsafeMutableRawPointer?, _ cb: (@convention(c) (UnsafeMutableRawPointer?, UnsafePointer<CChar>?) -> Void)?)

