#include "../include/OperacionesViewModel.h"
#include "../../Core/include/Helpers.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>

#define NAMESPACE Finexa::ViewModels
#define MY_VM_NAME OperacionesViewModel

namespace Finexa::ViewModels {

using namespace DualComponents::ViewModels;

std::shared_ptr<OperacionesViewModel>
OperacionesViewModel::create(std::shared_ptr<Finexa::CalculadoraCore> core) {
  return std::make_shared<OperacionesViewModel>(core);
}

std::shared_ptr<OperacionesViewModel> OperacionesViewModel::create() {
  return std::make_shared<OperacionesViewModel>(
      std::make_shared<Finexa::CalculadoraCore>());
}

OperacionesViewModel::OperacionesViewModel(
    std::shared_ptr<Finexa::CalculadoraCore> core)
    : _core(core) {

  _gridOperaciones = std::make_shared<DcGridViewModel>();
  _inputConcepto = std::make_shared<DcInputViewModel>();
  _inputMonto = std::make_shared<DcNumberFieldViewModel>();
  _selectorMoneda = std::make_shared<DcComboBoxViewModel>();
  _cmdAgregar = std::make_shared<DcCommandViewModel>();
  _cmdEliminar = std::make_shared<DcCommandViewModel>();
  _selectorMonedaRef = std::make_shared<DcComboBoxViewModel>();
  _labelTotal = std::make_shared<DcInputViewModel>();
  _labelMontoXds = std::make_shared<DcInputViewModel>();

  configurarColumnas();
  configurarMonedasDinamicas();

  // Wiring re-calculation
  _inputMonto->setOnValueChanged([this](int64_t) { this->recalcularXds(); });
  _selectorMoneda->setOnSelectionChanged(
      [this](std::string) { this->recalcularXds(); });

  // Configurar comandos
  _cmdAgregar->setLabelText("Registrar");
  _cmdAgregar->setOnExecuted([this]() { this->agregarOperacion(); });

  _cmdEliminar->setLabelText("Eliminar");
  _cmdEliminar->setOnExecuted([this]() { this->eliminarOperacion(); });

  // God Mode Debug
  _cmdCargarMock = std::make_shared<DcCommandViewModel>();
  _cmdCargarMock->setLabelText("Cargar Mock");
  _cmdCargarMock->setOnExecuted([this]() {
    _core->cargarDatosMock();
    this->inicializar(); // Recargar UI
  });

  _cmdLimpiarDB = std::make_shared<DcCommandViewModel>();
  _cmdLimpiarDB->setLabelText("Limpiar DB");
  _cmdLimpiarDB->setOnExecuted([this]() {
    _core->limpiarBaseDeDatos();
    this->inicializar(); // Recargar UI
  });

  // Cargar datos reales
  _core->cargarDesdeBD();
  inicializar();
}

// --- Implementación de accesores internos ---
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_GRID_OPERACIONES, Grid)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_INPUT_CONCEPTO, Input)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_INPUT_MONTO, NumberField)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_SELECTOR_MONEDA, ComboBox)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_CMD_AGREGAR, Command)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_CMD_ELIMINAR, Command)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_SELECTOR_MONEDA_REF,
                           ComboBox)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_LABEL_TOTAL, Input)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_LABEL_MONTO_XDS, Input)

// God Mode Internals
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, cmdCargarMock, Command)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, cmdLimpiarDB, Command)

// --- Internal Impl ---

bool OperacionesViewModel::tieneMonedas() {
  return !_core->getMonedas().empty();
}

bool OperacionesViewModel::isRefCurrencySelected() {
  return !_monedaRef.empty();
}

void OperacionesViewModel::configurarColumnas() {
  std::vector<DcGridColumn> cols = {
      {"Concepto", 150, DcTextAlign::Left, DcColumnType::Text},
      {"Monto", 100, DcTextAlign::Right, DcColumnType::Number},
      {"Moneda", 80, DcTextAlign::Center, DcColumnType::Text},
      {"Monto Ref", 120, DcTextAlign::Right, DcColumnType::Number}};
  _gridOperaciones->setColumns(cols);
}

void OperacionesViewModel::inicializar() {
  // Ya no inyectamos datos hardcoded aquí.
  // Los datos vienen de _core->cargarDesdeBD()

  // Cargar items de los ComboBoxes de moneda
  std::vector<DcComboBoxItem> items;
  for (const auto &m : _core->getMonedas()) {
    items.push_back({m->getSiglas(), m->getSiglas()});
  }
  _selectorMoneda->setItems(items);
  _selectorMonedaRef->setItems(items);

  // Seleccionar moneda referencial por defecto (primera moneda)
  if (!items.empty()) {
    _monedaRef = items[0].key;
    _selectorMonedaRef->selectItemByKey(_monedaRef);
  }

  // Cuando cambia la moneda referencial, recalcular todo
  _selectorMonedaRef->setOnSelectionChanged([this](std::string key) {
    _monedaRef = key;
    refrescarGrilla();
    recalcularXds();
  });

  // Configurar hooks de la grilla
  _gridOperaciones->setOnSelectionChanged(
      [this](int index) { this->cargarOperacion(index); });

  _gridOperaciones->setOnAddRequested([this]() { this->limpiarEditor(); });

  refrescarGrilla();
}

void OperacionesViewModel::refrescarDatos() {
  _core->cargarDesdeBD();
  inicializar();
}

// --- Helpers ---

double OperacionesViewModel::getMontoDouble() {
  return Finexa::getValorDouble(_inputMonto->getValue(),
                                _inputMonto->getDecimalPlaces());
}

double OperacionesViewModel::convertir(double monto, const std::string &from,
                                       const std::string &to) {
  if (from == to)
    return monto;

  std::string pivote = _core->getSiglasPivote();

  double tasaVesFrom = 1.0;
  if (from != pivote) {
    auto t = _core->buscarTasa(pivote, from);
    if (!t)
      return 0.0;
    tasaVesFrom = t->getValor();
  }

  double tasaVesTo = 1.0;
  if (to != pivote) {
    auto t = _core->buscarTasa(pivote, to);
    if (!t || t->getValor() == 0.0)
      return 0.0;
    tasaVesTo = t->getValor();
  }

  // Triangulación: monto(from) → VES → monto(to)
  double montoVes = monto * tasaVesFrom;
  return montoVes / tasaVesTo;
}

// --- Operaciones CRUD ---

void OperacionesViewModel::agregarOperacion() {
  std::string concepto = _inputConcepto->getText();
  double monto = getMontoDouble();
  std::string moneda = _selectorMoneda->getSelectedKey();

  if (!concepto.empty() && monto > 0 && !moneda.empty()) {
    auto m = _core->buscarMoneda(moneda);
    if (m) {
      double montoRef = convertir(monto, moneda, _monedaRef);

      if (_selectedIndex >= 0 &&
          _selectedIndex < static_cast<int>(_operaciones.size())) {
        // Edición
        auto op = _operaciones[_selectedIndex];
        op->setConcepto(concepto);
        op->setMontoOriginal(monto);
        op->setMonedaOriginal(m);
        op->setMontoXds(montoRef);
      } else {
        // Nuevo
        auto op =
            std::make_shared<Finexa::Operacion>(concepto, monto, m, montoRef);
        _operaciones.push_back(op);
      }

      limpiarEditor();
      refrescarGrilla();
    }
  }
}

void OperacionesViewModel::eliminarOperacion() {
  if (_selectedIndex >= 0 &&
      _selectedIndex < static_cast<int>(_operaciones.size())) {
    _operaciones.erase(_operaciones.begin() + _selectedIndex);
    limpiarEditor();
    refrescarGrilla();
  }
}

void OperacionesViewModel::cargarOperacion(int index) {
  if (index >= 0 && index < static_cast<int>(_operaciones.size())) {
    _selectedIndex = index;
    auto op = _operaciones[index];

    _inputConcepto->setText(op->getConcepto());

    // Convertir double a raw int64 para el NumberField
    int dp = _inputMonto->getDecimalPlaces();
    _inputMonto->setValue(Finexa::getValorRaw(op->getMontoOriginal(), dp));

    _selectorMoneda->selectItemByKey(op->getMonedaOriginal()->getSiglas());

    _cmdAgregar->setLabelText("Actualizar");
    _cmdEliminar->setVisible(true);
  }
}

void OperacionesViewModel::limpiarEditor() {
  _selectedIndex = -1;
  _inputConcepto->setText("");
  _inputMonto->setValue(0);

  _cmdAgregar->setLabelText("Registrar");
  _cmdEliminar->setVisible(false);

  recalcularXds();
}

void OperacionesViewModel::refrescarGrilla() {
  std::vector<DcGridRow> rows;
  double totalRef = 0.0;

  for (const auto &op : _operaciones) {
    DcGridRow row;
    std::stringstream ssMonto, ssRef;
    ssMonto << std::fixed << std::setprecision(2) << op->getMontoOriginal();

    // Convertir monto a moneda referencial
    double montoRef =
        convertir(op->getMontoOriginal(), op->getMonedaOriginal()->getSiglas(),
                  _monedaRef);
    ssRef << std::fixed << std::setprecision(2) << montoRef;
    totalRef += montoRef;

    // Actualizar el montoXds almacenado
    op->setMontoXds(montoRef);

    row.cells = {op->getConcepto(), ssMonto.str(),
                 op->getMonedaOriginal()->getSiglas(), ssRef.str()};
    rows.push_back(row);
  }
  _gridOperaciones->setRows(rows);

  // Actualizar Total con formato: Total(MONEDA): monto
  std::stringstream ssTotal;
  ssTotal << "Total(" << _monedaRef << "): " << std::fixed
          << std::setprecision(2) << totalRef;
  _labelTotal->setLabelText(ssTotal.str());
}

void OperacionesViewModel::configurarMonedasDinamicas() {
  _selectorMoneda->setAllowNew(true);
  _selectorMoneda->setOnNewItemRequested([this](std::string sigla) {
    auto nueva = std::make_shared<Finexa::Moneda>(0, sigla, sigla, "");
    _core->registrarMoneda(nueva);
    this->inicializar();
  });
}

void OperacionesViewModel::recalcularXds() {
  double monto = getMontoDouble();
  std::string moneda = _selectorMoneda->getSelectedKey();
  std::string label = "Monto en moneda Base: 0.00";

  if (!_monedaRef.empty()) {
    label = "Monto en moneda Base (" + _monedaRef + "): 0.00";
    if (monto > 0 && !moneda.empty()) {
      double montoRef = convertir(monto, moneda, _monedaRef);
      std::stringstream ss;
      ss << "Monto en moneda Base (" << _monedaRef << "): " << std::fixed
         << std::setprecision(2) << montoRef;
      label = ss.str();
    }
  }
  _labelMontoXds->setText(label);
}

} // namespace Finexa::ViewModels

// Implementación del C-Bridge
IMPLEMENT_VIEWMODEL_LIFECYCLE(NAMESPACE, MY_VM_NAME)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_GRID_OPERACIONES)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_INPUT_CONCEPTO)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_INPUT_MONTO)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_SELECTOR_MONEDA)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_CMD_AGREGAR)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_CMD_ELIMINAR)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_SELECTOR_MONEDA_REF)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_LABEL_TOTAL)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_LABEL_MONTO_XDS)

// God Mode Bridge
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, cmdCargarMock)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, cmdLimpiarDB)

DC_BRIDGE_EXPORT void OperacionesViewModel_cargarOperacion(void *vmPtr,
                                                           int index) {
  if (vmPtr) {
    auto vm = *static_cast<
        std::shared_ptr<Finexa::ViewModels::OperacionesViewModel> *>(vmPtr);
    vm->cargarOperacion(index);
  }
}

DC_BRIDGE_EXPORT void OperacionesViewModel_limpiarEditor(void *vmPtr) {
  if (vmPtr) {
    auto vm = *static_cast<
        std::shared_ptr<Finexa::ViewModels::OperacionesViewModel> *>(vmPtr);
    if (vm) {
      vm->limpiarEditor();
    }
  }
}

DC_BRIDGE_EXPORT bool OperacionesViewModel_tieneMonedas(void *vmPtr) {
  if (vmPtr) {
    auto vm = *static_cast<
        std::shared_ptr<Finexa::ViewModels::OperacionesViewModel> *>(vmPtr);
    if (vm) {
      return vm->tieneMonedas();
    }
  }
  return false;
}

DC_BRIDGE_EXPORT bool OperacionesViewModel_isRefCurrencySelected(void *vmPtr) {
  if (vmPtr) {
    auto vm = *static_cast<
        std::shared_ptr<Finexa::ViewModels::OperacionesViewModel> *>(vmPtr);
    if (vm) {
      return vm->isRefCurrencySelected();
    }
  }
  return false;
}

DC_BRIDGE_EXPORT void OperacionesViewModel_refrescarDatos(void *vmPtr) {
  if (vmPtr) {
    auto vm = *static_cast<
        std::shared_ptr<Finexa::ViewModels::OperacionesViewModel> *>(vmPtr);
    if (vm) {
      vm->refrescarDatos();
    }
  }
}
