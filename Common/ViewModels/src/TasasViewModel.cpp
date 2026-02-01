#include "../include/TasasViewModel.h"
#include <iomanip>
#include <sstream>

#define NAMESPACE Finexa::ViewModels
#define MY_VM_NAME TasasViewModel

namespace Finexa::ViewModels {

    using namespace DualComponents::ViewModels;

    std::shared_ptr<TasasViewModel> TasasViewModel::create(std::shared_ptr<Finexa::CalculadoraCore> core) {
        return std::make_shared<TasasViewModel>(core);
    }

    std::shared_ptr<TasasViewModel> TasasViewModel::create() {
        return std::make_shared<TasasViewModel>(std::make_shared<Finexa::CalculadoraCore>());
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

        _cmdGuardarTasa->setLabelText("Establecer Tasa");
        _cmdGuardarTasa->setOnExecuted([this]() {
            this->guardarTasa();
        });
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
            {"Valor", 100, DcTextAlign::Right, DcColumnType::Number}
        };
        _gridTasas->setColumns(cols);
    }

    void TasasViewModel::inicializar() {
        std::vector<DcComboBoxItem> items;
        for (const auto& m : _core->getMonedas()) {
            items.push_back({m->getSiglas(), m->getSiglas()});
        }
        _selectorBase->setItems(items);
        _selectorDestino->setItems(items);
        
        // Predeterminados según RF-03
        _selectorBase->selectItemByKey(_core->getSiglasPivote()); // VES

        refrescarGrilla();
    }

    void TasasViewModel::guardarTasa() {
        std::string base = _selectorBase->getSelectedKey();
        std::string destino = _selectorDestino->getSelectedKey();
        double valor = _inputValor->getValue();

        if (!base.empty() && !destino.empty() && base != destino && valor > 0) {
            // Logica de ambigüedad (RF-03)
            if (_core->tieneAmbiguedad(base, destino, valor)) {
                // TODO: Disparar alerta de ambigüedad si fuera necesario.
                // Por ahora procedemos a establecerla.
            }

            _core->establecerTasa(base, destino, valor);
            refrescarGrilla();
            _inputValor->setValue(0.0);
        }
    }

    void TasasViewModel::refrescarGrilla() {
        std::vector<DcGridRow> rows;
        for (const auto& t : _core->getTasas()) {
            DcGridRow row;
            std::stringstream ssValor;
            ssValor << std::fixed << std::setprecision(2) << t->getValor();

            row.cells = {
                t->getParidad(),
                ssValor.str()
            };
            rows.push_back(row);
        }
        _gridTasas->setRows(rows);
    }

    void TasasViewModel::configurarMonedasDinamicas() {
        auto handler = [this](std::string sigla) {
            // RF-01: Crear moneda automáticamente
            auto nueva = std::make_shared<Finexa::Moneda>(0, sigla, sigla, "");
            _core->registrarMoneda(nueva);
            
            // Refrescar combos
            this->inicializar(); 
        };

        _selectorBase->setAllowNew(true);
        _selectorBase->setOnNewItemRequested(handler);

        _selectorDestino->setAllowNew(true);
        _selectorDestino->setOnNewItemRequested(handler);
    }
} // namespace Finexa::ViewModels

// Implementación del C-Bridge
IMPLEMENT_VIEWMODEL_LIFECYCLE(NAMESPACE, MY_VM_NAME)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_GRID_TASAS)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_SELECTOR_BASE)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_SELECTOR_DESTINO)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_INPUT_VALOR)
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, MY_VM_NAME, VM_CMD_GUARDAR_TASA)
