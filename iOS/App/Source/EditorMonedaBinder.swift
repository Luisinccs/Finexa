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
    
    func bindFields(nombre: UITextField, simbolo: UITextField, siglas: UITextField) {
        // We need a mutable copy to get the pointer
        var vm = viewModel
        
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)

            // Bind Nombre
            if let ptr = MonedasViewModel_nombreViewModel(rawPtr) {
                (nombre as? DcTextField)?.setViewModel(ptr)
            }
            
            // Bind Simbolo
            if let ptr = MonedasViewModel_simboloViewModel(rawPtr) {
                (simbolo as? DcTextField)?.setViewModel(ptr)
            }
            
            // Bind Siglas
            if let ptr = MonedasViewModel_siglasViewModel(rawPtr) {
                (siglas as? DcTextField)?.setViewModel(ptr)
            }
        }
    }
    
    func bindLabels(lblNombre: UILabel, lblSimbolo: UILabel, lblSiglas: UILabel) {
        var vm = viewModel
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            if let ptr = MonedasViewModel_nombreViewModel(rawPtr) {
                lblNombre.setViewModel(ptr)
            }
            if let ptr = MonedasViewModel_simboloViewModel(rawPtr) {
                lblSimbolo.setViewModel(ptr)
            }
            if let ptr = MonedasViewModel_siglasViewModel(rawPtr) {
                lblSiglas.setViewModel(ptr)
            }
        }
    }
    
    func bindCommands(bar: DcCommandBar) {
        var vm = viewModel
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            // We assume DcCommandBar+Binder extension exists and bar.bind accepts Any
            // But we need to get the pointers.
            
            let acceptPtr = MonedasViewModel_aceptarViewModel(rawPtr)
            let cancelPtr = MonedasViewModel_cancelarViewModel(rawPtr)
            
            if let accept = acceptPtr, let cancel = cancelPtr {
                // Pass pointers as Any (or specific type if extension expects it)
                // The DcCommandBar extension expects Any and unwraps it.
                // We need to pass the raw pointer if possible or something the extension can handle.
                // The extension uses:
                // func setViewModel(_ sharedViewModel: Any)
                // which calls DcCommand_UnwrapToBinding(rawPtr from Any)
                // But here we have raw pointers directly from C bridge.
                // UIButton+DcBinder has: func setViewModel(_ viewModelPtr: UnsafeMutableRawPointer, ...)
                // DcCommandBar extension calls btn.setViewModel(any).
                
                // We should update DcCommandBar extension to accept UnsafeMutableRawPointer or make sure we pass something compatible.
                // Or we call bar.btnAccept.setViewModel(accept) directly here.
                // That might be safer given standard Swift C-interop is tricky with "Any".
                
                bar.btnAccept.setViewModel(accept)
                bar.btnCancel.setViewModel(cancel)
            }
            
            // Bind navigation if we had commands for it, but seemingly we don't in VM yet.
            // Or we handle focus navigation locally in Controller?
            // "los botones de aceptar y cancelar, deben poder enlazarse con command view models"
            // "tab next y tab prev" -> usually local.
        }
    }
    
    func bindCloseRequest(handler: @escaping () -> Void) {
        var vm = viewModel
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            // Helper wrapper to keep closure alive
            let closureWrapper = Unmanaged.passRetained(ClosureWrapper(closure: handler)).toOpaque()
            
            // Assuming this function exists similar to Operaciones
            MonedasViewModel_setOnRequestClose(rawPtr, closureWrapper) { ctx in
                guard let ctx = ctx else { return }
                let wrapper = Unmanaged<ClosureWrapper>.fromOpaque(ctx).takeUnretainedValue()
                DispatchQueue.main.async {
                    wrapper.closure()
                }
            }
        }
    }
    
    func bindDialog(to viewController: UIViewController) {
        print("[EditorMonedaBinder] Binding dialog...")
        var vm = viewModel
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let dialogPtr = MonedasViewModel_dialogViewModel(rawPtr) {
                print("[EditorMonedaBinder] Found dialogViewModel pointer, calling setViewModel")
                DcDialogBinder.setViewModel(dialogPtr, to: viewController)
            } else {
                print("[EditorMonedaBinder] Error: MonedasViewModel_dialogViewModel returned nil")
            }
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

