#include "MonedasViewModel.hpp"
#include <iostream>

namespace Finexa::ViewModels {

MonedasViewModel::MonedasViewModel(
    std::shared_ptr<Finexa::CalculadoraCore> core)
    : _core(core) {
  // Init properties via macro-generated members
  _monedasGridViewModel = DcGridViewModel::create();

  _nombreViewModel = std::make_shared<DcInputViewModel>();
  _simboloViewModel = std::make_shared<DcInputViewModel>();
  _siglasViewModel = std::make_shared<DcInputViewModel>();

  _aceptarViewModel = std::make_shared<DcCommandViewModel>();
  _cancelarViewModel = std::make_shared<DcCommandViewModel>();
  _eliminarViewModel = std::make_shared<DcCommandViewModel>();

  setup();
}

MonedasViewModel::~MonedasViewModel() {}

std::shared_ptr<MonedasViewModel>
MonedasViewModel::create(std::shared_ptr<Finexa::CalculadoraCore> core) {
  return std::make_shared<MonedasViewModel>(core);
}

std::shared_ptr<MonedasViewModel> MonedasViewModel::create() {
  return std::make_shared<MonedasViewModel>(
      std::make_shared<Finexa::CalculadoraCore>());
}

void MonedasViewModel::setup() {
  setupGrid();
  configureEditors();
  setupEditingLogic();

  // Cargar datos del core (si no se han cargado)
  // Nota: Esto recarga desde la BD. Si ya se cargó en Operaciones, es
  // redundante pero seguro.
  _core->cargarDesdeBD();

  refreshRows();
}

void MonedasViewModel::setupGrid() {
  std::vector<DcGridColumn> cols;

  // 0: Nombre (Input)
  cols.push_back(
      DcGridColumn("Nombre", 40, DcTextAlign::Left, DcColumnType::Text));
  // 1: Siglas (Input)
  cols.push_back(
      DcGridColumn("Siglas", 20, DcTextAlign::Center, DcColumnType::Text));
  // 2: Simbolo (Input)
  cols.push_back(
      DcGridColumn("Símbolo", 20, DcTextAlign::Center, DcColumnType::Text));
  // 3: Actions
  cols.push_back(
      DcGridColumn("Acciones", 20, DcTextAlign::Center, DcColumnType::Actions));

  _monedasGridViewModel->setColumns(cols);
}

void MonedasViewModel::configureEditors() {
  _nombreViewModel->setLabelText("Nombre");
  _simboloViewModel->setLabelText("Símbolo");
  _siglasViewModel->setLabelText("Siglas");
}

void MonedasViewModel::setupEditingLogic() {
  // 1. Activation
  auto startEdit = [this](int index) {
    std::shared_ptr<Moneda> item = nullptr;
    const auto &monedasList = _core->getMonedas();

    if (index >= 0 && index < static_cast<int>(monedasList.size())) {
      item = monedasList[index];
    }

    // Lock Grid
    _monedasGridViewModel->setIsInputLocked(true);
    currentEditingItem = item; // Store for referencing during save if needed

    if (item) {
      isNewItem = false;
      _nombreViewModel->setText(item->getNombre());
      _simboloViewModel->setText(item->getSimbolo());
      _siglasViewModel->setText(item->getSiglas());
    } else {
      isNewItem = true;
      _nombreViewModel->setText("");
      _simboloViewModel->setText("");
      _siglasViewModel->setText("");
    }
  };

  _monedasGridViewModel->setOnRowActivated(
      [this, startEdit](int index) { startEdit(index); });
  _monedasGridViewModel->setOnAddRequested(
      [this, startEdit]() { startEdit(-1); });

  // 2. Save (Aceptar)
  _aceptarViewModel->setOnExecuted([this]() {
    std::string nombre = _nombreViewModel->getText();
    std::string simbolo = _simboloViewModel->getText();
    std::string siglas = _siglasViewModel->getText();

    if (nombre.empty() || siglas.empty()) {
      return; // Validation failed
    }

    if (isNewItem) {
      // Create new
      int nextId = 0; // ID no longer primarily used for logic, UUID is key
      // ID auto-generation is database generated usually, or we pass 0
      auto newItem = std::make_shared<Moneda>(nextId, siglas, nombre, simbolo);
      _core->guardarMoneda(newItem);
    } else {
      // Edit existing
      if (currentEditingItem) {
        currentEditingItem->setNombre(nombre);
        currentEditingItem->setSimbolo(simbolo);
        currentEditingItem->setSiglas(
            siglas); // Changing primary key/siglas might require careful
                     // handling if used as FK
        _core->guardarMoneda(currentEditingItem);
      }
    }

    _monedasGridViewModel->setIsInputLocked(false);
    refreshRows();
  });

  // 3. Cancel
  _cancelarViewModel->setOnExecuted(
      [this]() { _monedasGridViewModel->setIsInputLocked(false); });

  // 4. Delete
  _eliminarViewModel->setOnExecuted([this]() {
    if (isNewItem) {
      _monedasGridViewModel->setIsInputLocked(false);
      return;
    }

    if (currentEditingItem) {
      _core->eliminarMoneda(currentEditingItem->getUuid());
    }

    _monedasGridViewModel->setIsInputLocked(false);
    refreshRows();
  });
}

void MonedasViewModel::refreshRows() {
  std::vector<DcGridRow> rows;
  const auto &monedasList = _core->getMonedas();

  for (auto &item : monedasList) {
    DcGridRow r;
    r.tag = item.get();

    // Columns: Nombre, Siglas, Simbolo, Acciones
    r.cells.push_back(item->getNombre());
    r.cells.push_back(item->getSiglas());
    r.cells.push_back(item->getSimbolo());
    r.cells.push_back(""); // Actions placeholder

    rows.push_back(r);
  }

  // Ghost row
  rows.push_back(DcGridRow::CreateGhost(4));

  _monedasGridViewModel->setRows(rows);
}

// =========================================================
// Accessor Implementations (Macro-based)
// =========================================================

IMPLEMENT_CONTROL_INTERNAL(Finexa::ViewModels, MONEDAS_VIEW_MODEL, GRID_MONEDAS,
                           Grid)

IMPLEMENT_CONTROL_INTERNAL(Finexa::ViewModels, MONEDAS_VIEW_MODEL, TXT_NOMBRE,
                           Input)
IMPLEMENT_CONTROL_INTERNAL(Finexa::ViewModels, MONEDAS_VIEW_MODEL, TXT_SIMBOLO,
                           Input)
IMPLEMENT_CONTROL_INTERNAL(Finexa::ViewModels, MONEDAS_VIEW_MODEL, TXT_SIGLAS,
                           Input)

IMPLEMENT_CONTROL_INTERNAL(Finexa::ViewModels, MONEDAS_VIEW_MODEL, CMD_ACEPTAR,
                           Command)
IMPLEMENT_CONTROL_INTERNAL(Finexa::ViewModels, MONEDAS_VIEW_MODEL, CMD_CANCELAR,
                           Command)
IMPLEMENT_CONTROL_INTERNAL(Finexa::ViewModels, MONEDAS_VIEW_MODEL, CMD_ELIMINAR,
                           Command)

} // namespace Finexa::ViewModels

// =========================================================
// C-Bridge Function Implementations
// =========================================================

extern "C" {

IMPLEMENT_VIEWMODEL_LIFECYCLE(Finexa::ViewModels, MONEDAS_VIEW_MODEL)

DC_BRIDGE_EXPORT void *MonedasViewModel_monedasGridViewModel(void *vmPtr) {
  if (!vmPtr)
    return nullptr;
  auto vm =
      *static_cast<std::shared_ptr<Finexa::ViewModels::MonedasViewModel> *>(
          vmPtr);
  auto *binding = vm->monedasGridViewModelBinding();
  return static_cast<IGridBinding *>(binding);
}

IMPLEMENT_CONTROL_BRIDGE(Finexa::ViewModels, MONEDAS_VIEW_MODEL,
                         nombreViewModel)
IMPLEMENT_CONTROL_BRIDGE(Finexa::ViewModels, MONEDAS_VIEW_MODEL,
                         simboloViewModel)
IMPLEMENT_CONTROL_BRIDGE(Finexa::ViewModels, MONEDAS_VIEW_MODEL,
                         siglasViewModel)

IMPLEMENT_CONTROL_BRIDGE(Finexa::ViewModels, MONEDAS_VIEW_MODEL,
                         aceptarViewModel)
IMPLEMENT_CONTROL_BRIDGE(Finexa::ViewModels, MONEDAS_VIEW_MODEL,
                         cancelarViewModel)
IMPLEMENT_CONTROL_BRIDGE(Finexa::ViewModels, MONEDAS_VIEW_MODEL,
                         eliminarViewModel)

} // extern "C"
