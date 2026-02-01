#include "../include/FinexaMainViewModel.h"

#define NAMESPACE Finexa::ViewModels
#define MY_VM_NAME FinexaMainViewModel

namespace Finexa::ViewModels {

    std::shared_ptr<FinexaMainViewModel> FinexaMainViewModel::create(std::shared_ptr<Finexa::CalculadoraCore> core) {
        return std::make_shared<FinexaMainViewModel>(core);
    }

    std::shared_ptr<FinexaMainViewModel> FinexaMainViewModel::create() {
        return std::make_shared<FinexaMainViewModel>(std::make_shared<Finexa::CalculadoraCore>());
    }

    FinexaMainViewModel::FinexaMainViewModel(std::shared_ptr<Finexa::CalculadoraCore> core)
        : _core(core) {
        
        _operacionesVm = OperacionesViewModel::create(core);
        _tasasVm = TasasViewModel::create(core);
    }

    std::shared_ptr<OperacionesViewModel> FinexaMainViewModel::getOperacionesVm() const {
        return _operacionesVm;
    }

    std::shared_ptr<TasasViewModel> FinexaMainViewModel::getTasasVm() const {
        return _tasasVm;
    }

    void FinexaMainViewModel::inicializar() {
        // Inicializar datos predeterminados en el Core
        _core->registrarMoneda(std::make_shared<Finexa::Moneda>(1, "VES", "Bolívares", "Bs."));
        _core->registrarMoneda(std::make_shared<Finexa::Moneda>(2, "USD", "Dólares", "$"));
        _core->registrarMoneda(std::make_shared<Finexa::Moneda>(3, "EUR", "Euros", "€"));
        _core->registrarMoneda(std::make_shared<Finexa::Moneda>(4, "XDS", "Moneda Objetivo", "XDS"));

        // Tasas de ejemplo (RF-02)
        _core->establecerTasa("VES", "USD", 330.0);
        _core->establecerTasa("VES", "EUR", 380.0);
        _core->establecerTasa("VES", "XDS", 550.0);

        // Inicializar sub-viewmodels
        _operacionesVm->inicializar();
        _tasasVm->inicializar();
    }

} // namespace Finexa::ViewModels

// Implementación del C-Bridge
IMPLEMENT_VIEWMODEL_LIFECYCLE(NAMESPACE, MY_VM_NAME)
