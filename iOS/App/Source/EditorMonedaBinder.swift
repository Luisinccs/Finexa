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
    
    func bind(nombre: UITextField, simbolo: UITextField, siglas: UITextField) {
        // We need a mutable copy to get the pointer
        var vm = viewModel
        
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            // Bind Nombre
            if let ptr = MonedasViewModel_nombreViewModel(rawPtr) {
                (nombre as? HaloTextField)?.setHaloTextFieldViewModel(ptr)
            }
            
            // Bind Simbolo
            if let ptr = MonedasViewModel_simboloViewModel(rawPtr) {
                (simbolo as? HaloTextField)?.setHaloTextFieldViewModel(ptr)
            }
            
            // Bind Siglas
            if let ptr = MonedasViewModel_siglasViewModel(rawPtr) {
                (siglas as? HaloTextField)?.setHaloTextFieldViewModel(ptr)
            }
        }
    }
    
    func accept() {
        var vm = viewModel
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let cmdPtr = MonedasViewModel_aceptarViewModel(rawPtr) {
                DcCommand_Execute(cmdPtr)
            }
        }
    }
    
    func cancel() {
        var vm = viewModel
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            if let cmdPtr = MonedasViewModel_cancelarViewModel(rawPtr) {
                DcCommand_Execute(cmdPtr)
            }
        }
    }
}

// MARK: - Extensions for Custom Controls
extension HaloTextField {
    /// Wrapper to use the standard setViewModel with a specific name for HaloTextField, 
    /// as suggested by the USER to avoid shadowing issues.
    func setHaloTextFieldViewModel(_ ptr: UnsafeMutableRawPointer) {
        self.setViewModel(ptr)
    }
}
