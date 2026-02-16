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
    
    public func bind(base: DcComboBox, destino: DcComboBox, valor: DcNumberTextField) {
        var vm = viewModel
        
        withUnsafeMutablePointer(to: &vm) { vmPtr in
            let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
            
            if let ptr = TasasViewModel_selectorBase(rawPtr) {
                base.bindViewModel(ptr)
            }
            if let ptr = TasasViewModel_selectorDestino(rawPtr) {
                destino.bindViewModel(ptr)
            }
            if let ptr = TasasViewModel_inputValor(rawPtr) {
                valor.bindViewModel(ptr)
            }
        }
    }
    
    public func save() {
         var vm = viewModel
         withUnsafeMutablePointer(to: &vm) { vmPtr in
             let rawPtr = UnsafeMutableRawPointer(mutating: vmPtr)
             if let cmdPtr = TasasViewModel_cmdGuardarTasa(rawPtr) {
                 DcCommand_Execute(cmdPtr)
             }
         }
    }
    
    public func cancel() {
        // No cancel command exposed in C++ for TasasViewModel yet?
        // Revisando TasasViewModel.h... 
        // DECLARE_CONTROL_BINDING(VM_CMD_GUARDAR_TASA, Command)
        // Solo guardar. Cancelar suele ser solo cerrar la vista en este caso si no hay lógica compleja.
    }
}
