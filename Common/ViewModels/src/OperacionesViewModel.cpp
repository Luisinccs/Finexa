#include "../include/OperacionesViewModel.h"
#include <iomanip>
#include <sstream>

#define NAMESPACE Finexa::ViewModels
#define MY_VM_NAME OperacionesViewModel

namespace Finexa::ViewModels {

    using namespace DualComponents::ViewModels;

    std::shared_ptr<OperacionesViewModel> OperacionesViewModel::create(std::shared_ptr<Finexa::CalculadoraCore> core) {
        return std::make_shared<OperacionesViewModel>(core);
    }

    std::shared_ptr<OperacionesViewModel> OperacionesViewModel::create() {
        return std::make_shared<OperacionesViewModel>(std::make_shared<Finexa::CalculadoraCore>());
    }

    OperacionesViewModel::OperacionesViewModel(std::shared_ptr<Finexa::CalculadoraCore> core)
        : _core(core) {
        
        _gridOperaciones = std::make_shared<DcGridViewModel>();
        _inputConcepto = std::make_shared<DcInputViewModel>();
        _inputMonto = std::make_shared<DcNumberFieldViewModel>();
        _selectorMoneda = std::make_shared<DcComboBoxViewModel>();
        _cmdAgregar = std::make_shared<DcCommandViewModel>();

        configurarColumnas();
        configurarMonedasDinamicas();

        _cmdAgregar->setLabelText("Registrar");
        _cmdAgregar->setOnExecuted([this]() {
            this->agregarOperacion();
        });
    }

    // --- Implementación de accesores internos ---
    IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_GRID_OPERACIONES, Grid)
    IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_INPUT_CONCEPTO, Input)
    IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_INPUT_MONTO, NumberField)
    IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_SELECTOR_MONEDA, ComboBox)
    IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, MY_VM_NAME, VM_CMD_AGREGAR, Command)

    void OperacionesViewModel::configurarColumnas() {
        std::vector<DcGridColumn> cols = {
            {"Concepto", 150, DcTextAlign::Left, DcColumnType::Text},
            {"Monto", 100, DcTextAlign::Right, DcColumnType::Number},
            {"Moneda", 80, DcTextAlign::Center, DcColumnType::Text},
            {"Monto XDS", 120, DcTextAlign::Right, DcColumnType::Number}
        };
        _gridOperaciones->setColumns(cols);
    }

    void OperacionesViewModel::inicializar() {
        std::vector<DcComboBoxItem> items;
        for (const auto& m : _core->getMonedas()) {
            items.push_back({m->getSiglas(), m->getSiglas()});
        }
        _selectorMoneda->setItems(items);
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
                auto op = std::make_shared<Finexa::Operacion>(concepto, monto, m, xds);
                _operaciones.push_back(op);

                // Limpiar editor
                _inputConcepto->setText("");
                _inputMonto->setValue(0.0);

                refrescarGrilla();
            }
        }
    }

    void OperacionesViewModel::refrescarGrilla() {
        std::vector<DcGridRow> rows;
        for (const auto& op : _operaciones) {
            DcGridRow row;
            std::stringstream ssMonto, ssXds;
            ssMonto << std::fixed << std::setprecision(2) << op->getMontoOriginal();
            ssXds << std::fixed << std::setprecision(2) << op->getMontoXds();

            row.cells = {
                op->getConcepto(),
                ssMonto.str(),
                op->getMonedaOriginal()->getSiglas(),
                ssXds.str()
            };
            rows.push_back(row);
        }
        _gridOperaciones->setRows(rows);
    }

    void OperacionesViewModel::configurarMonedasDinamicas() {
        _selectorMoneda->setAllowNew(true);
        _selectorMoneda->setOnNewItemRequested([this](std::string sigla) {
            auto nueva = std::make_shared<Finexa::Moneda>(0, sigla, sigla, "");
            _core->registrarMoneda(nueva);
            this->inicializar();
        });
    }

} // namespace Finexa::ViewModels

// Implementación del C-Bridge
IMPLEMENT_VIEWMODEL_LIFECYCLE(NAMESPACE, MY_VM_NAME)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_GRID_OPERACIONES)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_INPUT_CONCEPTO)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_INPUT_MONTO)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_SELECTOR_MONEDA)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_CMD_AGREGAR)
