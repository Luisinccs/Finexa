#include "../include/OperacionesViewModel.h"
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
  _labelMonedaRef = std::make_shared<DcInputViewModel>();
  _labelTotal = std::make_shared<DcInputViewModel>();
  _labelMontoXds = std::make_shared<DcInputViewModel>();

  configurarColumnas();
  configurarMonedasDinamicas();

  // Wiring re-calculation
  _inputMonto->setOnValueChanged([this](double) { this->recalcularXds(); });
  _selectorMoneda->setOnSelectionChanged(
      [this](std::string) { this->recalcularXds(); });

  // Configurar comandos
  _cmdAgregar->setLabelText("Registrar");
  _cmdAgregar->setOnExecuted([this]() { this->agregarOperacion(); });

  _cmdEliminar->setLabelText("Eliminar");
  _cmdEliminar->setOnExecuted([this]() { this->eliminarOperacion(); });

  inicializar();
}

// --- Implementación de accesores internos ---
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_GRID_OPERACIONES, Grid)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_INPUT_CONCEPTO, Input)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_INPUT_MONTO, NumberField)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_SELECTOR_MONEDA, ComboBox)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_CMD_AGREGAR, Command)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_CMD_ELIMINAR, Command)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_LABEL_MONEDA_REF, Input)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_LABEL_TOTAL, Input)
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_LABEL_MONTO_XDS, Input)

void OperacionesViewModel::configurarColumnas() {
  std::vector<DcGridColumn> cols = {
      {"Concepto", 150, DcTextAlign::Left, DcColumnType::Text},
      {"Monto", 100, DcTextAlign::Right, DcColumnType::Number},
      {"Moneda", 80, DcTextAlign::Center, DcColumnType::Text},
      {"Monto XDS", 120, DcTextAlign::Right, DcColumnType::Number}};
  _gridOperaciones->setColumns(cols);
}

void OperacionesViewModel::inicializar() {
  std::vector<DcComboBoxItem> items;
  for (const auto &m : _core->getMonedas()) {
    items.push_back({m->getSiglas(), m->getSiglas()});
  }

  // Configurar hooks de la grilla
  _gridOperaciones->setOnSelectionChanged(
      [this](int index) { this->cargarOperacion(index); });

  _gridOperaciones->setOnAddRequested([this]() { this->limpiarEditor(); });

  // Configurar labels iniciales
  auto monedaRef = _core->getSiglasPivote();
  _labelMonedaRef->setLabelText("Referencia: " + monedaRef);

  refrescarGrilla();
}

void OperacionesViewModel::agregarOperacion() {
  std::string concepto = _inputConcepto->getText();
  double monto = _inputMonto->getValue();
  std::string moneda = _selectorMoneda->getSelectedKey();

  if (!concepto.empty() && monto > 0 && !moneda.empty()) {
    auto m = _core->buscarMoneda(moneda);
    if (m) {
      double xds = _core->convertirAXds(monto, moneda);

      if (_selectedIndex >= 0 && _selectedIndex < _operaciones.size()) {
        // Edición
        auto op = _operaciones[_selectedIndex];
        op->setConcepto(concepto);
        op->setMontoOriginal(monto);
        op->setMonedaOriginal(m);
        op->setMontoXds(xds);
      } else {
        // Nuevo
        auto op = std::make_shared<Finexa::Operacion>(concepto, monto, m, xds);
        _operaciones.push_back(op);
      }

      limpiarEditor();
      refrescarGrilla();
    }
  }
}

void OperacionesViewModel::eliminarOperacion() {
  if (_selectedIndex >= 0 && _selectedIndex < _operaciones.size()) {
    _operaciones.erase(_operaciones.begin() + _selectedIndex);
    limpiarEditor();
    refrescarGrilla();
  }
}

void OperacionesViewModel::cargarOperacion(int index) {
  if (index >= 0 && index < _operaciones.size()) {
    _selectedIndex = index;
    auto op = _operaciones[index];

    _inputConcepto->setText(op->getConcepto());
    _inputMonto->setValue(op->getMontoOriginal());
    _selectorMoneda->selectItemByKey(op->getMonedaOriginal()->getSiglas());

    _cmdAgregar->setLabelText("Actualizar");
    _cmdEliminar->setVisible(true); // Mostrar botón eliminar en edición
  }
}

void OperacionesViewModel::limpiarEditor() {
  _selectedIndex = -1;
  _inputConcepto->setText("");
  _inputMonto->setValue(0.0);
  // Reset selector? Opcional

  _cmdAgregar->setLabelText("Registrar");
  _cmdEliminar->setVisible(false); // Ocultar al limpiar (modo nuevo)

  recalcularXds();
}

void OperacionesViewModel::refrescarGrilla() {
  std::vector<DcGridRow> rows;
  double totalXds = 0.0;

  for (const auto &op : _operaciones) {
    DcGridRow row;
    std::stringstream ssMonto, ssXds;
    ssMonto << std::fixed << std::setprecision(2) << op->getMontoOriginal();
    ssXds << std::fixed << std::setprecision(2) << op->getMontoXds();

    totalXds += op->getMontoXds();

    row.cells = {op->getConcepto(), ssMonto.str(),
                 op->getMonedaOriginal()->getSiglas(), ssXds.str()};
    rows.push_back(row);
  }
  _gridOperaciones->setRows(rows);

  // Actualizar Total
  std::stringstream ssTotal;
  ssTotal << "Total: " << std::fixed << std::setprecision(2) << totalXds;
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
  double monto = _inputMonto->getValue();
  std::string moneda = _selectorMoneda->getSelectedKey();
  std::string label = "REF: 0.00";

  if (monto > 0 && !moneda.empty()) {
    double xds = _core->convertirAXds(monto, moneda);
    std::stringstream ss;
    ss << "REF: " << std::fixed << std::setprecision(2) << xds << " "
       << _core->getSiglasPivote();
    label = ss.str();
  }
  _labelMontoXds->setLabelText(label);
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
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_LABEL_MONEDA_REF)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_LABEL_TOTAL)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_LABEL_MONTO_XDS)

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
    vm->limpiarEditor();
  }
}
