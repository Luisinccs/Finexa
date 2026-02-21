#pragma once

#define VM_GRID_TASAS gridTasas
#define VM_SELECTOR_BASE selectorBase
#define VM_SELECTOR_DESTINO selectorDestino
#define VM_INPUT_VALOR inputValor
#define VM_CMD_GUARDAR_TASA cmdGuardarTasa
#define VM_CMD_CANCELAR cmdCancelar
#define VM_CMD_ELIMINAR cmdEliminar
#define VM_DIALOG dialog

#include "DcComboBoxViewModel.hpp"
#include "DcCommandViewModel.hpp"
#include "DcDialogViewModel.hpp" // Added
#include "DcGridViewModel.hpp"
#include "DcNumberFieldViewModel.hpp"
#include "DcViewModelToolkit.hpp"

#include "IComboBoxBinding.hpp"
#include "ICommandBinding.hpp"
#include "IDialogBinding.hpp" // Added
#include "IGridBinding.hpp"
#include "INumberFieldBinding.hpp"

#include "../../Core/include/CalculadoraCore.h"

#include <functional> // Added for std::function
#include <memory>

namespace Finexa::ViewModels {

class TasasViewModel {
  // Declaración de bindings
  DECLARE_CONTROL_BINDING(VM_GRID_TASAS, Grid)
  DECLARE_CONTROL_BINDING(VM_SELECTOR_BASE, ComboBox)
  DECLARE_CONTROL_BINDING(VM_SELECTOR_DESTINO, ComboBox)
  DECLARE_CONTROL_BINDING(VM_INPUT_VALOR, NumberField)
  DECLARE_CONTROL_BINDING(VM_CMD_GUARDAR_TASA, Command)
  DECLARE_CONTROL_BINDING(VM_CMD_CANCELAR, Command)
  DECLARE_CONTROL_BINDING(VM_CMD_ELIMINAR, Command)
  DECLARE_CONTROL_BINDING(VM_DIALOG, Dialog)

public: // Moved setOnRequestClose to public as per instruction
  void setOnRequestClose(std::function<void()> callback) {
    _onRequestClose = callback;
  }

private:
  std::shared_ptr<Finexa::CalculadoraCore> _core;
  std::function<void()> _onRequestClose;
  int _selectedIndex = -1;

public:
  TasasViewModel(std::shared_ptr<Finexa::CalculadoraCore> core);
  virtual ~TasasViewModel() = default;

  static std::shared_ptr<TasasViewModel>
  create(std::shared_ptr<Finexa::CalculadoraCore> core);
  static std::shared_ptr<TasasViewModel> create();

  void inicializar();
  bool guardarTasa();
  void eliminarTasa();
  void refrescarGrilla();

private:
  void configurarColumnas();
  void configurarMonedasDinamicas();
};

using TasasViewModelPtr = std::shared_ptr<TasasViewModel>;

} // namespace Finexa::ViewModels

// Declaraciones para el C-Bridge
// Declaraciones para el C-Bridge
extern "C" {
DECLARE_VIEWMODEL_LIFECYCLE(TasasViewModel)
DECLARE_CONTROL_BRIDGE(VM_GRID_TASAS, TasasViewModel)
DECLARE_CONTROL_BRIDGE(VM_SELECTOR_BASE, TasasViewModel)
DECLARE_CONTROL_BRIDGE(VM_SELECTOR_DESTINO, TasasViewModel)
DECLARE_CONTROL_BRIDGE(VM_INPUT_VALOR, TasasViewModel)
DECLARE_CONTROL_BRIDGE(VM_CMD_GUARDAR_TASA, TasasViewModel)
DECLARE_CONTROL_BRIDGE(VM_CMD_CANCELAR, TasasViewModel)
DECLARE_CONTROL_BRIDGE(VM_CMD_ELIMINAR, TasasViewModel)
DECLARE_CONTROL_BRIDGE(VM_DIALOG, TasasViewModel)

extern "C" {
DC_BRIDGE_EXPORT void TasasViewModel_setOnRequestClose(void *vmPtr, void *ctx,
                                                       void (*cb)(void *));
}
}
