#pragma once

#define VM_GRID_OPERACIONES gridOperaciones
#define VM_INPUT_CONCEPTO inputConcepto
#define VM_INPUT_MONTO inputMonto
#define VM_SELECTOR_MONEDA selectorMoneda
#define VM_CMD_AGREGAR cmdAgregar
#define VM_CMD_ELIMINAR cmdEliminar
#define VM_SELECTOR_MONEDA_REF selectorMonedaRef
#define VM_LABEL_TOTAL labelTotal
#define VM_LABEL_MONTO_XDS labelMontoXds

#include "DcComboBoxViewModel.hpp"
#include "DcCommandViewModel.hpp"
#include "DcGridViewModel.hpp"
#include "DcInputViewModel.hpp"
#include "DcNumberFieldViewModel.hpp"
#include "DcViewModelToolkit.hpp"

#include "IComboBoxBinding.hpp"
#include "ICommandBinding.hpp"
#include "IGridBinding.hpp"
#include "IInputBinding.hpp"
#include "INumberFieldBinding.hpp"

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
  DECLARE_CONTROL_BINDING(VM_CMD_ELIMINAR, Command)
  DECLARE_CONTROL_BINDING(VM_SELECTOR_MONEDA_REF, ComboBox)
  DECLARE_CONTROL_BINDING(VM_LABEL_TOTAL, Input)
  DECLARE_CONTROL_BINDING(VM_LABEL_MONTO_XDS, Input)

  // God Mode Commands
  DECLARE_CONTROL_BINDING(cmdCargarMock, Command)
  DECLARE_CONTROL_BINDING(cmdLimpiarDB, Command)

private:
  std::shared_ptr<Finexa::CalculadoraCore> _core;
  std::vector<std::shared_ptr<Finexa::Operacion>> _operaciones;
  int _selectedIndex = -1; // -1 indicates new operation
  std::string _monedaRef;  // Moneda referencial seleccionada

public:
  OperacionesViewModel(std::shared_ptr<Finexa::CalculadoraCore> core);
  virtual ~OperacionesViewModel() = default;

  static std::shared_ptr<OperacionesViewModel>
  create(std::shared_ptr<Finexa::CalculadoraCore> core);
  static std::shared_ptr<OperacionesViewModel> create();

  void inicializar();
  void agregarOperacion();
  void eliminarOperacion();
  void cargarOperacion(int index);
  void limpiarEditor();
  void refrescarGrilla();
  void refrescarDatos();

  bool tieneMonedas();          // Helper for Empty State
  bool isRefCurrencySelected(); // Helper for Coach Marks

private:
  void configurarColumnas();
  void configurarMonedasDinamicas();
  void recalcularXds();
  double getMontoDouble();
  double convertir(double monto, const std::string &from,
                   const std::string &to);
};

using OperacionesViewModelPtr = std::shared_ptr<OperacionesViewModel>;

} // namespace Finexa::ViewModels

// Declaraciones para el C-Bridge
extern "C" {
DECLARE_VIEWMODEL_LIFECYCLE(OperacionesViewModel)
DECLARE_CONTROL_BRIDGE(VM_GRID_OPERACIONES, OperacionesViewModel)
DECLARE_CONTROL_BRIDGE(VM_INPUT_CONCEPTO, OperacionesViewModel)
DECLARE_CONTROL_BRIDGE(VM_INPUT_MONTO, OperacionesViewModel)
DECLARE_CONTROL_BRIDGE(VM_SELECTOR_MONEDA, OperacionesViewModel)
DECLARE_CONTROL_BRIDGE(VM_CMD_AGREGAR, OperacionesViewModel)
DECLARE_CONTROL_BRIDGE(VM_CMD_ELIMINAR, OperacionesViewModel)
DECLARE_CONTROL_BRIDGE(VM_SELECTOR_MONEDA_REF, OperacionesViewModel)
DECLARE_CONTROL_BRIDGE(VM_LABEL_TOTAL, OperacionesViewModel)
DECLARE_CONTROL_BRIDGE(VM_LABEL_MONTO_XDS, OperacionesViewModel)

DECLARE_CONTROL_BRIDGE(cmdCargarMock, OperacionesViewModel)
DECLARE_CONTROL_BRIDGE(cmdLimpiarDB, OperacionesViewModel)

DC_BRIDGE_EXPORT void OperacionesViewModel_cargarOperacion(void *vmPtr,
                                                           int index);
DC_BRIDGE_EXPORT void OperacionesViewModel_limpiarEditor(void *vmPtr);
DC_BRIDGE_EXPORT bool OperacionesViewModel_tieneMonedas(void *vmPtr);
DC_BRIDGE_EXPORT void OperacionesViewModel_refrescarDatos(void *vmPtr);
}
