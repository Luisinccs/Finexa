import UIKit
import DcViewModels
import DcViewsIos
import FinexaViewsIos

// MARK: - Concrete Implementation

/// Concrete implementation of the binder using DcViewModels.
class EditorMonedaAppBinder: EditorMonedaBinder {
    
    private var viewModel: DualComponents.TestHelpers.MonedasViewModelPtr
    
    init(viewModel: DualComponents.TestHelpers.MonedasViewModelPtr) {
        self.viewModel = viewModel
    }
    
    func bind(nombre: UITextField, simbolo: UITextField, siglas: UITextField) {
        // We need a mutable copy to get the pointer
        var vm = viewModel
        
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            // Bind Nombre
            if let ptr = MonedasViewModel_nombreViewModel(rawPtr) {
                if let halo = nombre as? HaloTextField {
                    halo.setViewModel(ptr)
                } else {
                    // Fallback if safe or if extension exists on UITextField
                    // Assuming setViewModel is available on UITextField via DcViewsIos extension
                    // If not, we might need to cast or ensure HaloTextField is used
                    (nombre as? HaloTextField)?.setViewModel(ptr)
                }
            }
            
            // Bind Simbolo
            if let ptr = MonedasViewModel_simboloViewModel(rawPtr) {
                (simbolo as? HaloTextField)?.setViewModel(ptr)
            }
            
            // Bind Siglas
            if let ptr = MonedasViewModel_siglasViewModel(rawPtr) {
                (siglas as? HaloTextField)?.setViewModel(ptr)
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
