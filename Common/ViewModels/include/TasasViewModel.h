#pragma once

#define VM_GRID_TASAS gridTasas
#define VM_SELECTOR_BASE selectorBase
#define VM_SELECTOR_DESTINO selectorDestino
#define VM_INPUT_VALOR inputValor
#define VM_CMD_GUARDAR_TASA cmdGuardarTasa

#include "DcViewModelToolkit.hpp"
#include "DcGridViewModel.hpp"
#include "DcCommandViewModel.hpp"
#include "DcNumberFieldViewModel.hpp"
#include "DcComboBoxViewModel.hpp"

#include "IGridBinding.hpp"
#include "ICommandBinding.hpp"
#include "INumberFieldBinding.hpp"
#include "IComboBoxBinding.hpp"

#include "../../Core/include/CalculadoraCore.h"

#include <memory>

namespace Finexa::ViewModels {

class TasasViewModel {
    // Declaración de bindings
    DECLARE_CONTROL_BINDING(VM_GRID_TASAS, Grid)
    DECLARE_CONTROL_BINDING(VM_SELECTOR_BASE, ComboBox)
    DECLARE_CONTROL_BINDING(VM_SELECTOR_DESTINO, ComboBox)
    DECLARE_CONTROL_BINDING(VM_INPUT_VALOR, NumberField)
    DECLARE_CONTROL_BINDING(VM_CMD_GUARDAR_TASA, Command)

private:
    std::shared_ptr<Finexa::CalculadoraCore> _core;

public:
    TasasViewModel(std::shared_ptr<Finexa::CalculadoraCore> core);
    virtual ~TasasViewModel() = default;

    static std::shared_ptr<TasasViewModel> create(std::shared_ptr<Finexa::CalculadoraCore> core);
    static std::shared_ptr<TasasViewModel> create();

    void inicializar();
    void guardarTasa();
    void refrescarGrilla();

private:
    void configurarColumnas();
    void configurarMonedasDinamicas();
};

} // namespace Finexa::ViewModels

// Declaraciones para el C-Bridge
DECLARE_VIEWMODEL_LIFECYCLE(TasasViewModel)
DECLARE_CONTROL_BRIDGE(VM_GRID_TASAS, TasasViewModel)
DECLARE_CONTROL_BRIDGE(VM_SELECTOR_BASE, TasasViewModel)
DECLARE_CONTROL_BRIDGE(VM_SELECTOR_DESTINO, TasasViewModel)
DECLARE_CONTROL_BRIDGE(VM_INPUT_VALOR, TasasViewModel)
DECLARE_CONTROL_BRIDGE(VM_CMD_GUARDAR_TASA, TasasViewModel)
