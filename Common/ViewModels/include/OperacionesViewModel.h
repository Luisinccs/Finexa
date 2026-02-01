#pragma once

#define VM_GRID_OPERACIONES gridOperaciones
#define VM_INPUT_CONCEPTO inputConcepto
#define VM_INPUT_MONTO inputMonto
#define VM_SELECTOR_MONEDA selectorMoneda
#define VM_CMD_AGREGAR cmdAgregar

#include "DcViewModelToolkit.hpp"
#include "DcGridViewModel.hpp"
#include "DcCommandViewModel.hpp"
#include "DcNumberFieldViewModel.hpp"
#include "DcComboBoxViewModel.hpp"
#include "DcInputViewModel.hpp"

#include "IGridBinding.hpp"
#include "ICommandBinding.hpp"
#include "INumberFieldBinding.hpp"
#include "IComboBoxBinding.hpp"
#include "IInputBinding.hpp"

#include "../../Core/include/CalculadoraCore.h"
#include "../../Core/include/Operacion.h"

#include <memory>
#include <vector>

namespace Finexa::ViewModels {

class OperacionesViewModel {
    // Declaración de bindings
    DECLARE_CONTROL_BINDING(VM_GRID_OPERACIONES, Grid)
    DECLARE_CONTROL_BINDING(VM_INPUT_CONCEPTO, Input)
    DECLARE_CONTROL_BINDING(VM_INPUT_MONTO, NumberField)
    DECLARE_CONTROL_BINDING(VM_SELECTOR_MONEDA, ComboBox)
    DECLARE_CONTROL_BINDING(VM_CMD_AGREGAR, Command)

private:
    std::shared_ptr<Finexa::CalculadoraCore> _core;
    std::vector<std::shared_ptr<Finexa::Operacion>> _operaciones;

public:
    OperacionesViewModel(std::shared_ptr<Finexa::CalculadoraCore> core);
    virtual ~OperacionesViewModel() = default;

    static std::shared_ptr<OperacionesViewModel> create(std::shared_ptr<Finexa::CalculadoraCore> core);
    static std::shared_ptr<OperacionesViewModel> create();

    void inicializar();
    void agregarOperacion();
    void refrescarGrilla();

private:
    void configurarColumnas();
    void configurarMonedasDinamicas();
};

} // namespace Finexa::ViewModels

// Declaraciones para el C-Bridge
DECLARE_VIEWMODEL_LIFECYCLE(OperacionesViewModel)
DECLARE_CONTROL_BRIDGE(VM_GRID_OPERACIONES, OperacionesViewModel)
DECLARE_CONTROL_BRIDGE(VM_INPUT_CONCEPTO, OperacionesViewModel)
DECLARE_CONTROL_BRIDGE(VM_INPUT_MONTO, OperacionesViewModel)
DECLARE_CONTROL_BRIDGE(VM_SELECTOR_MONEDA, OperacionesViewModel)
DECLARE_CONTROL_BRIDGE(VM_CMD_AGREGAR, OperacionesViewModel)
