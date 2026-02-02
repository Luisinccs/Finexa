#include "../include/TasasViewModel.h"
#include <iomanip>
#include <sstream>

#define NAMESPACE Finexa::ViewModels
#define MY_VM_NAME TasasViewModel

namespace Finexa::ViewModels {

using namespace DualComponents::ViewModels;

std::shared_ptr<TasasViewModel>
TasasViewModel::create(std::shared_ptr<Finexa::CalculadoraCore> core) {
  return std::make_shared<TasasViewModel>(core);
}

std::shared_ptr<TasasViewModel> TasasViewModel::create() {
  return std::make_shared<TasasViewModel>(
      std::make_shared<Finexa::CalculadoraCore>());
}

TasasViewModel::TasasViewModel(std::shared_ptr<Finexa::CalculadoraCore> core)
    : _core(core) {

  _gridTasas = std::make_shared<DcGridViewModel>();
  _selectorBase = std::make_shared<DcComboBoxViewModel>();
  _selectorDestino = std::make_shared<DcComboBoxViewModel>();
  _inputValor = std::make_shared<DcNumberFieldViewModel>();
  _cmdGuardarTasa = std::make_shared<DcCommandViewModel>();

  configurarColumnas();
  configurarMonedasDinamicas();

  cmdGuardarTasa()->setLabelText("Establecer Tasa");
  //        _cmdGuardarTasa->setLabelText("Establecer Tasa");
  cmdGuardarTasa()->setOnExecuted([this]() { this->guardarTasa(); });
}

// --- Implementación de accesores internos ---
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_GRID_TASAS, Grid)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_SELECTOR_BASE, ComboBox)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_SELECTOR_DESTINO, ComboBox)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_INPUT_VALOR, NumberField)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_CMD_GUARDAR_TASA, Command)

void TasasViewModel::configurarColumnas() {
  std::vector<DcGridColumn> cols = {
      {"Paridad", 120, DcTextAlign::Left, DcColumnType::Text},
      {"Valor", 100, DcTextAlign::Right, DcColumnType::Number}};
  _gridTasas->setColumns(cols);
}

void TasasViewModel::inicializar() {
  // DEBUG: Inject Test Data
  if (_core->getMonedas().empty()) {
    auto usd =
        std::make_shared<Finexa::Moneda>(1, "USD", "USD", "Dolar Americano");
    _core->registrarMoneda(usd);
    auto ves = std::make_shared<Finexa::Moneda>(2, "VES", "VES", "Bolivar");
    _core->registrarMoneda(ves);
  }

  std::vector<DcComboBoxItem> items;
  for (const auto &m : _core->getMonedas()) {
    items.push_back({m->getSiglas(), m->getSiglas()});
  }
  selectorBase()->setItems(items);
  selectorDestino()->setItems(items);

  // Predeterminados según RF-03
  selectorBase()->selectItemByKey(_core->getSiglasPivote()); // VES

  refrescarGrilla();
}

void TasasViewModel::guardarTasa() {
  std::string base = selectorBase()->getSelectedKey();
  std::string destino = selectorDestino()->getSelectedKey();
  double valor = inputValor()->getValue();

  if (!base.empty() && !destino.empty() && base != destino && valor > 0) {
    // Logica de ambigüedad (RF-03)
    if (_core->tieneAmbiguedad(base, destino, valor)) {
      // TODO: Disparar alerta de ambigüedad si fuera necesario.
      // Por ahora procedemos a establecerla.
    }

    _core->establecerTasa(base, destino, valor);
    refrescarGrilla();
    inputValor()->setValue(0.0);
  }
}

void TasasViewModel::refrescarGrilla() {
  std::vector<DcGridRow> rows;
  for (const auto &t : _core->getTasas()) {
    DcGridRow row;
    std::stringstream ssValor;
    ssValor << std::fixed << std::setprecision(2) << t->getValor();

    row.cells = {t->getParidad(), ssValor.str()};
    rows.push_back(row);
  }
  gridTasas()->setRows(rows);
}

void TasasViewModel::configurarMonedasDinamicas() {
  auto handler = [this](std::string sigla) {
    // RF-01: Crear moneda automáticamente
    auto nueva = std::make_shared<Finexa::Moneda>(0, sigla, sigla, "");
    _core->registrarMoneda(nueva);

    // Refrescar combos
    this->inicializar();
  };

  selectorBase()->setAllowNew(true);
  selectorBase()->setOnNewItemRequested(handler);

  selectorDestino()->setAllowNew(true);
  selectorDestino()->setOnNewItemRequested(handler);
}
} // namespace Finexa::ViewModels

// Implementación del C-Bridge
IMPLEMENT_VIEWMODEL_LIFECYCLE(NAMESPACE, MY_VM_NAME)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_GRID_TASAS)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_SELECTOR_BASE)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_SELECTOR_DESTINO)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_INPUT_VALOR)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_CMD_GUARDAR_TASA)
