#pragma once

#include "DcViewModelToolkit.hpp"
#include "../../Core/include/CalculadoraCore.h"
#include "OperacionesViewModel.h"
#include "TasasViewModel.h"

#include <memory>

namespace Finexa::ViewModels {

class FinexaMainViewModel {
private:
    std::shared_ptr<Finexa::CalculadoraCore> _core;
    
    // Sub-ViewModels
    std::shared_ptr<OperacionesViewModel> _operacionesVm;
    std::shared_ptr<TasasViewModel> _tasasVm;

public:
    FinexaMainViewModel(std::shared_ptr<Finexa::CalculadoraCore> core);
    virtual ~FinexaMainViewModel() = default;

    static std::shared_ptr<FinexaMainViewModel> create(std::shared_ptr<Finexa::CalculadoraCore> core);
    static std::shared_ptr<FinexaMainViewModel> create();

    std::shared_ptr<OperacionesViewModel> getOperacionesVm() const;
    std::shared_ptr<TasasViewModel> getTasasVm() const;

    void inicializar();
};

} // namespace Finexa::ViewModels

// Declaraciones para el C-Bridge
DECLARE_VIEWMODEL_LIFECYCLE(FinexaMainViewModel)
