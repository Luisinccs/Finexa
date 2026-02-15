/**
 * @file MonedasViewModel.hpp
 * @date 2026-02-05
 * @brief ViewModel para la gestión de Monedas.
 */

#pragma once

#define NOMBRE_VIEW_MODEL MonedasViewModel

#define GRD_MONEDAS gridMonedas
#define TXT_NOMBRE txtNombre
#define TXT_SIGLAS txtSiglas
#define TXT_SIMBOLO txtSimbolo
// Comandos estandar para grid form
#define CMD_ACEPTAR cmdAceptar
#define CMD_CANCELAR cmdCancelar

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "DcViewModelToolkit.hpp"
#include "DcVmFactory.hpp"
#include "IDcBindings.hpp"
#include "Moneda.h"

using namespace DualComponents::ViewModels;

namespace MiApp::ViewModels {

/**
 * @brief ViewModel para la administración del catálogo de Monedas.
 */
class NOMBRE_VIEW_MODEL {
  
  // =========================================================
  // Declaraciones de view models de controles
  // =========================================================
  DECLARE_CONTROL_BINDING(GRD_MONEDAS, Grid)
  DECLARE_CONTROL_BINDING(TXT_NOMBRE, Input)
  DECLARE_CONTROL_BINDING(TXT_SIGLAS, Input)
  DECLARE_CONTROL_BINDING(TXT_SIMBOLO, Input)
  
  DECLARE_CONTROL_BINDING(CMD_ACEPTAR, Command)
  DECLARE_CONTROL_BINDING(CMD_CANCELAR, Command)

private:
  // Colección de monedas
  std::vector<std::shared_ptr<Finexa::Moneda>> _monedas;
  
  // Helpers para edición
  std::shared_ptr<Finexa::Moneda> currentEditingItem;
  bool isNewItem = false;

  void setUpControls();
  void setupGrid();
  void setupEditingLogic();
  void refreshRows();

public:
  NOMBRE_VIEW_MODEL();
  virtual ~NOMBRE_VIEW_MODEL() = default;
  static std::shared_ptr<NOMBRE_VIEW_MODEL> create();
};

using NOMBRE_VIEW_MODEL_PTR = std::shared_ptr<NOMBRE_VIEW_MODEL>;

} // namespace MiApp::ViewModels

// =========================================================
// C-Bridge Functions
// =========================================================

DECLARE_VIEWMODEL_LIFECYCLE(NOMBRE_VIEW_MODEL)

DECLARE_CONTROL_BRIDGE(GRD_MONEDAS, NOMBRE_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(TXT_NOMBRE, NOMBRE_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(TXT_SIGLAS, NOMBRE_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(TXT_SIMBOLO, NOMBRE_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(CMD_ACEPTAR, NOMBRE_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(CMD_CANCELAR, NOMBRE_VIEW_MODEL)
