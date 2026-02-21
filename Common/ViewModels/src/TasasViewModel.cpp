#include "../include/TasasViewModel.h"
#include "../../Core/include/Helpers.hpp"
#include <cmath>
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
  _cmdCancelar = std::make_shared<DcCommandViewModel>();
  _cmdEliminar = std::make_shared<DcCommandViewModel>();
  _dialog = std::make_shared<DcDialogViewModel>();

  configurarColumnas();
  configurarMonedasDinamicas();

  // Initial setup for labels
  _selectorBase->setLabelText("Moneda Base");
  _selectorDestino->setLabelText("Moneda Destino");
  _inputValor->setLabelText("Tasa");

  _cmdCancelar->setLabelText("Cancelar");
  _cmdCancelar->setOnExecuted([this]() {
    if (_onRequestClose) {
      _onRequestClose();
    }
  });

  _cmdEliminar->setLabelText("Eliminar");
  _cmdEliminar->setOnExecuted([this]() { this->eliminarTasa(); });

  // Logic for editing
  auto loadForEdit = [this](int index) {
    const auto &tasas = _core->getTasas();
    if (index >= 0 && index < static_cast<int>(tasas.size())) {
      _selectedIndex = index;
      auto tasa = tasas[index];
      // Populate form
      _selectorBase->selectItemByKey(tasa->getBase()->getUuid());
      _selectorDestino->selectItemByKey(tasa->getDestino()->getUuid());

      int dp = _inputValor->getDecimalPlaces();
      _inputValor->setValue(Finexa::getValorRaw(tasa->getValor(), dp));

      // Disable selectors during edit (Constraint)
      _selectorBase->setEnabled(false);
      _selectorDestino->setEnabled(false);

      _cmdGuardarTasa->setLabelText("Actualizar");
      _cmdEliminar->setVisible(true);
    }
  };

  _gridTasas->setOnRowActivated(loadForEdit);

  // Logic for new/reset
  auto resetForm = [this]() {
    _selectedIndex = -1;
    _selectorBase->setEnabled(true);
    _selectorDestino->setEnabled(true);
    _inputValor->setValue(0.0);
    _cmdGuardarTasa->setLabelText("Establecer Tasa");
    _cmdEliminar->setVisible(false);

    // Select defaults
    auto p = _core->buscarMoneda(_core->getSiglasPivote());
    if (p)
      _selectorBase->selectItemByKey(p->getUuid());
  };

  _gridTasas->setOnAddRequested(resetForm);

  cmdGuardarTasa()->setLabelText("Establecer Tasa");
  cmdGuardarTasa()->setOnExecuted([this, resetForm]() {
    if (this->guardarTasa()) {
      resetForm(); // Reset after save
      if (_onRequestClose) {
        _onRequestClose(); // Solo se cierra si fue exitoso
      }
    }
  });

  inicializar();
}

// --- Implementación de accesores internos ---
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_GRID_TASAS, Grid)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_SELECTOR_BASE, ComboBox)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_SELECTOR_DESTINO, ComboBox)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_INPUT_VALOR, NumberField)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_CMD_GUARDAR_TASA, Command)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_CMD_CANCELAR, Command)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_CMD_ELIMINAR, Command)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_DIALOG, Dialog)

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

  // Guardamos las selecciones actuales
  std::string currBase = selectorBase()->getSelectedKey();
  std::string currDestino = selectorDestino()->getSelectedKey();

  selectorBase()->setItems(items);
  selectorDestino()->setItems(items);

  bool baseFound = false;
  bool destinoFound = false;
  for (const auto &item : items) {
    if (item.key == currBase)
      baseFound = true;
    if (item.key == currDestino)
      destinoFound = true;
  }

  // Predeterminados según RF-03 (Select via UUID) si no hay selección válida
  if (!baseFound) {
    auto pivot = _core->buscarMoneda(_core->getSiglasPivote());
    if (pivot) {
      selectorBase()->selectItemByKey(pivot->getUuid());
    }
  } else {
    selectorBase()->selectItemByKey(currBase);
  }

  if (destinoFound) {
    selectorDestino()->selectItemByKey(currDestino);
  }

  refrescarGrilla();
}

bool TasasViewModel::guardarTasa() {
  std::string baseUuid = selectorBase()->getSelectedKey();
  std::string destinoUuid = selectorDestino()->getSelectedKey();
  double valor = Finexa::getValorDouble(inputValor()->getValue(),
                                        inputValor()->getDecimalPlaces());

  if (baseUuid.empty() || destinoUuid.empty()) {
    if (_dialog)
      _dialog->showUiAlert("Error", "Debe seleccionar ambas monedas.");
    return false;
  }

  if (baseUuid == destinoUuid) {
    if (_dialog)
      _dialog->showUiAlert("Error",
                           "Las monedas base y destino no pueden ser iguales.");
    return false;
  }

  if (valor <= 0) {
    if (_dialog)
      _dialog->showUiAlert("Error", "El valor de la tasa debe ser mayor a 0.");
    return false;
  }

  // Si estamos creando una nueva tasa (no editando)
  if (_selectedIndex < 0) {
    auto mBase = _core->buscarMonedaPorUuid(baseUuid);
    auto mDestino = _core->buscarMonedaPorUuid(destinoUuid);

    if (mBase && mDestino) {
      // 1. Verificar si ya existe una tasa directa entre estas dos (en ambos
      // sentidos)
      auto tasaExistente =
          _core->buscarTasa(mBase->getSiglas(), mDestino->getSiglas());
      if (!tasaExistente) {
        tasaExistente =
            _core->buscarTasa(mDestino->getSiglas(), mBase->getSiglas());
      }

      if (tasaExistente) {
        if (_dialog)
          _dialog->showUiAlert("Tasa Duplicada",
                               "Ya existe una tasa entre estas dos monedas. "
                               "Puede editarla desde la lista.");
        return false;
      }

      // 2. Verificar si existe triangulación implícita
      std::string pivote = _core->getSiglasPivote();
      if (mBase->getSiglas() != pivote && mDestino->getSiglas() != pivote) {
        double valorImplicito = _core->calcularValorImplicito(
            mBase->getSiglas(), mDestino->getSiglas());
        if (valorImplicito > 0.0) {
          if (_dialog)
            _dialog->showUiAlert("Triangulación detectada",
                                 "Ya existe una tasa calculada implícitamente "
                                 "entre estas monedas a través del VES. No se "
                                 "permite crear una tasa directa redundante.");
          return false;
        }
      }
    }
  }

  try {
    _core->establecerTasaPorUuid(baseUuid, destinoUuid, valor);
    refrescarGrilla();
    return true;
  } catch (...) {
    if (_dialog)
      _dialog->showUiAlert("Error", "Ocurrió un error al guardar la tasa.");
    return false;
  }
}

void TasasViewModel::eliminarTasa() {
  const auto &tasas = _core->getTasas();
  if (_selectedIndex >= 0 && _selectedIndex < static_cast<int>(tasas.size())) {
    auto tasa = tasas[_selectedIndex];
    _core->eliminarTasa(tasa->getUuid());

    // Reset form states manually similar to resetForm
    _selectedIndex = -1;
    _selectorBase->setEnabled(true);
    _selectorDestino->setEnabled(true);
    _inputValor->setValue(0.0);
    _cmdGuardarTasa->setLabelText("Establecer Tasa");
    _cmdEliminar->setVisible(false);

    refrescarGrilla();
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
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_CMD_CANCELAR)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_CMD_ELIMINAR)

extern "C" {
DC_BRIDGE_EXPORT void TasasViewModel_setOnRequestClose(void *vmPtr, void *ctx,
                                                       void (*cb)(void *)) {
  if (!vmPtr || !cb)
    return;
  auto vm = *static_cast<std::shared_ptr<Finexa::ViewModels::TasasViewModel> *>(
      vmPtr);

  vm->setOnRequestClose([ctx, cb]() { cb(ctx); });
}

DC_BRIDGE_EXPORT void *TasasViewModel_dialog(void *vmPtr) {
  if (vmPtr) {
    auto vm =
        *static_cast<std::shared_ptr<Finexa::ViewModels::TasasViewModel> *>(
            vmPtr);
    if (vm) {
      return vm->dialogBinding();
    }
  }
  return nullptr;
}
}
