#pragma once

#include "DcCommandViewModel.hpp"
#include "DcControlViewModel.hpp"
#include "DcGridViewModel.hpp"
#include "DcInputViewModel.hpp"
#include "DcViewModelToolkit.hpp"
#include <memory>
#include <string>
#include <vector>

#include "../../Core/include/CalculadoraCore.h"

// Definition of class and namespace
#define MONEDAS_VIEW_MODEL MonedasViewModel

// Member Macros
#define GRID_MONEDAS monedasGridViewModel
#define TXT_NOMBRE nombreViewModel
#define TXT_SIMBOLO simboloViewModel
#define TXT_SIGLAS siglasViewModel
#define CMD_ACEPTAR aceptarViewModel
#define CMD_CANCELAR cancelarViewModel
#define CMD_ELIMINAR eliminarViewModel

namespace Finexa::ViewModels {

using namespace DualComponents::ViewModels;

class MonedasViewModel {
public:
  MonedasViewModel(std::shared_ptr<Finexa::CalculadoraCore> core);
  virtual ~MonedasViewModel();

  static std::shared_ptr<MonedasViewModel>
  create(std::shared_ptr<Finexa::CalculadoraCore> core);
  static std::shared_ptr<MonedasViewModel> create();

  void setup();

  // --- Declarations ---
  DECLARE_CONTROL_BINDING(GRID_MONEDAS, Grid)

  DECLARE_CONTROL_BINDING(TXT_NOMBRE, Input)
  DECLARE_CONTROL_BINDING(TXT_SIMBOLO, Input)
  DECLARE_CONTROL_BINDING(TXT_SIGLAS, Input)

  DECLARE_CONTROL_BINDING(CMD_ACEPTAR, Command)
  DECLARE_CONTROL_BINDING(CMD_CANCELAR, Command)
  DECLARE_CONTROL_BINDING(CMD_ELIMINAR, Command)

private:
  // Data
  std::shared_ptr<Finexa::CalculadoraCore> _core;

  // Helpers
  std::shared_ptr<Moneda> currentEditingItem;
  bool isNewItem = false;

  void setupGrid();
  void configureEditors();
  void setupEditingLogic();
  void refreshRows();
};

using MonedasViewModelPtr = std::shared_ptr<MonedasViewModel>;

} // namespace Finexa::ViewModels

// =========================================================
// C-Bridge Functions
// =========================================================

extern "C" {

DECLARE_VIEWMODEL_LIFECYCLE(MONEDAS_VIEW_MODEL)

DECLARE_CONTROL_BRIDGE(monedasGridViewModel, MONEDAS_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(nombreViewModel, MONEDAS_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(simboloViewModel, MONEDAS_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(siglasViewModel, MONEDAS_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(aceptarViewModel, MONEDAS_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(cancelarViewModel, MONEDAS_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(eliminarViewModel, MONEDAS_VIEW_MODEL)

// Helper bridge used in Swift
DC_BRIDGE_EXPORT void DcGrid_RequestAdd(void *gridPtr);

} // extern "C"
