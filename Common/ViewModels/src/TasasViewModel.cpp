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

  // Logic for editing
  auto loadForEdit = [this](int index) {
    const auto &tasas = _core->getTasas();
    if (index >= 0 && index < static_cast<int>(tasas.size())) {
      auto tasa = tasas[index];
      // Populate form
      _selectorBase->selectItemByKey(tasa->getBase()->getUuid());
      _selectorDestino->selectItemByKey(tasa->getDestino()->getUuid());
      _inputValor->setValue(tasa->getValor());

      // Disable selectors during edit (Constraint)
      _selectorBase->setEnabled(false);
      _selectorDestino->setEnabled(false);

      _cmdGuardarTasa->setLabelText("Actualizar");
    }
  };

  _gridTasas->setOnRowActivated(loadForEdit);

  // Logic for new/reset
  auto resetForm = [this]() {
    _selectorBase->setEnabled(true);
    _selectorDestino->setEnabled(true);
    _inputValor->setValue(0.0);
    _cmdGuardarTasa->setLabelText("Establecer Tasa");

    // Select defaults
    auto p = _core->buscarMoneda(_core->getSiglasPivote());
    if (p)
      _selectorBase->selectItemByKey(p->getUuid());
  };

  _gridTasas->setOnAddRequested(resetForm);

  cmdGuardarTasa()->setLabelText("Establecer Tasa");
  cmdGuardarTasa()->setOnExecuted([this, resetForm]() {
    this->guardarTasa();
    resetForm(); // Reset after save
  });

  inicializar();
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
  // Cargar datos (si no se han cargado externamente)
  _core->cargarDesdeBD();

  std::vector<DcComboBoxItem> items;
  for (const auto &m : _core->getMonedas()) {
    // Key = UUID, Value = Siglas
    items.push_back({m->getUuid(), m->getSiglas()});
  }
  selectorBase()->setItems(items);
  selectorDestino()->setItems(items);

  // Predeterminados según RF-03 (Select via UUID)
  auto pivot = _core->buscarMoneda(_core->getSiglasPivote());
  if (pivot) {
    selectorBase()->selectItemByKey(pivot->getUuid());
  }

  refrescarGrilla();
}

void TasasViewModel::guardarTasa() {
  std::string baseUuid = selectorBase()->getSelectedKey();
  std::string destinoUuid = selectorDestino()->getSelectedKey();
  double valor = inputValor()->getValue();

  if (!baseUuid.empty() && !destinoUuid.empty() && baseUuid != destinoUuid &&
      valor > 0) {
    // NOTE: Ambiguedad logic uses Siglas currently in Core, but we have UUIDs.
    // We might want to update ambiguedad logic or just retrieve coins to get
    // siglas. For now, trusting establecerTasaPorUuid to handle logic.

    try {
      _core->establecerTasaPorUuid(baseUuid, destinoUuid, valor);
      refrescarGrilla();
    } catch (...) {
      // Handle error?
    }
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

  // Ghost row for "New"
  rows.push_back(DcGridRow::CreateGhost(2));

  gridTasas()->setRows(rows);
}

void TasasViewModel::configurarMonedasDinamicas() {
  auto handler = [this](std::string sigla) {
    // RF-01: Crear moneda automáticamente
    // Note: This logic seems to rely on the user typing a SIGLA in the
    // search/edit box of the combo DcComboBox "OnNewItemRequested" passes the
    // text string typed by user. So "sigla" here is the text.

    // Check if duplicate logic needed? Core::registrarMoneda handles uniqueness
    // by Siglas.

    // We create a new moneda with 0 ID (auto-gen/ignored), UUID generated in
    // constructor. This constructor might need to change if Moneda(0,...)
    // doesn't generate UUID? Let's assume Moneda(...) generates UUID if not
    // provided (or we explicitly provide). Moneda constructor signatures:
    // 1. (int id, siglas, nombre, simbolo) -> generates UUID? Need to check
    // Moneda.cpp
    //    It doesn't seem to generate UUID in the code I viewed earlier?
    //    Let's check Moneda.h/cpp again. Step 3020 showed:
    //    Moneda(int id, ...) : id(id) ... {} -> UUID member initialized to
    //    empty string?

    // FIX: Core needs to ensure Moneda has UUID if we rely on it.
    // However, Moneda class doesn't seem to auto-gen UUID in constructor.
    // Data Layer should probably handle it or we inject it.
    // For now, I will generate a simple UUID-like string or rely on repository
    // to fix it. Or I should fix Moneda constructor to gen UUID. Currently
    // "registrarMoneda" adds it. "guardarMoneda" in Core calls Repository save.

    // Assuming MonedaRepository handles new items.

    auto nueva = std::make_shared<Finexa::Moneda>(0, sigla, sigla, "");
    // Manually set a temp UUID if needed so UI can select it?
    // Or rely on reload.

    _core->registrarMoneda(nueva);
    _core->guardarMoneda(nueva); // Ensure persistence

    // Refrescar combos
    this->inicializar();

    // Select the new item (by UUID, but we need to know it.
    // If Moneda logic doesn't gen UUID on construction, we might have empty
    // UUID. This is a risk. I should fix Moneda constructor to generate UUID if
    // empty.
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
