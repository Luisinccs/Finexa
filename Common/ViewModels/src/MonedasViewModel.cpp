/**
 * @file MonedasViewModel.cpp
 * @date 2026-02-05
 * @brief Implementación del ViewModel para la gestión de Monedas.
 */

#include "MonedasViewModel.hpp"

namespace MiApp::ViewModels {

// =========================================================
// Ciclo de vida
// =========================================================

std::shared_ptr<NOMBRE_VIEW_MODEL> NOMBRE_VIEW_MODEL::create() {
  return std::make_shared<NOMBRE_VIEW_MODEL>();
}

NOMBRE_VIEW_MODEL::NOMBRE_VIEW_MODEL() {
  // Creación de view models de controles
  _gridMonedas = DcGridViewModel::create();
  _txtNombre = createInputViewModelPtr();
  _txtSiglas = createInputViewModelPtr();
  _txtSimbolo = createInputViewModelPtr();
  
  _cmdAceptar = createCommandViewModelPtr();
  _cmdCancelar = createCommandViewModelPtr();

  setUpControls();
  
  // Agregar datos de prueba si la colección está vacía
  if (_monedas.empty()) {
      // Datos ficticios iniciales
      _monedas.push_back(std::make_shared<Finexa::Moneda>(1, "USD", "Dolar Estadounidense", "$"));
      _monedas.push_back(std::make_shared<Finexa::Moneda>(2, "EUR", "Euro", "€"));
      _monedas.push_back(std::make_shared<Finexa::Moneda>(3, "DOP", "Peso Dominicano", "RD$"));
  }
  
  refreshRows();
}

// =========================================================
// Definiciones de accesores de bindings
// =========================================================

IMPLEMENT_CONTROL_INTERNAL(MiApp::ViewModels, NOMBRE_VIEW_MODEL, GRD_MONEDAS, Grid);
IMPLEMENT_CONTROL_INTERNAL(MiApp::ViewModels, NOMBRE_VIEW_MODEL, TXT_NOMBRE, Input);
IMPLEMENT_CONTROL_INTERNAL(MiApp::ViewModels, NOMBRE_VIEW_MODEL, TXT_SIGLAS, Input);
IMPLEMENT_CONTROL_INTERNAL(MiApp::ViewModels, NOMBRE_VIEW_MODEL, TXT_SIMBOLO, Input);
IMPLEMENT_CONTROL_INTERNAL(MiApp::ViewModels, NOMBRE_VIEW_MODEL, CMD_ACEPTAR, Command);
IMPLEMENT_CONTROL_INTERNAL(MiApp::ViewModels, NOMBRE_VIEW_MODEL, CMD_CANCELAR, Command);

// =========================================================
// Funciones privadas de utileria
// =========================================================

void NOMBRE_VIEW_MODEL::setUpControls() {
  setupGrid();
  
  // Configuración de comandos
  cmdAceptar()->setLabelText("Guardar");
  cmdCancelar()->setLabelText("Cancelar Edición");
  
  // Configuración inicial de editores
  txtNombre()->setLabelText("Nombre");
  txtSiglas()->setLabelText("Siglas");
  txtSimbolo()->setLabelText("Símbolo");
  
  setupEditingLogic();
}

void NOMBRE_VIEW_MODEL::setupGrid() {
  std::vector<DcGridColumn> cols;
  
  // Configuración de columnas
  // Siglas | Nombre | Símbolo | Op
  cols.push_back(DcGridColumn("Siglas", 5, DcTextAlign::Center));
  cols.push_back(DcGridColumn("Nombre", 20, DcTextAlign::Left));
  cols.push_back(DcGridColumn("Símbolo", 5, DcTextAlign::Center));
  cols.push_back(DcGridColumn("Op", 8, DcTextAlign::Center, DcColumnType::Actions));

  _gridMonedas->setColumns(cols);
}

void NOMBRE_VIEW_MODEL::setupEditingLogic() {
  
  // Lógica de inicio de edición (selección o nuevo)
  auto startEdit = [this](int index) {
    Finexa::Moneda* item = nullptr;
    
    if (index >= 0 && index < _monedas.size()) {
        item = _monedas[index].get();
    }
    
    // Bloquear grid durante edición
    _gridMonedas->setIsInputLocked(true);
    currentEditingItem.reset();
    
    if (item) {
        // Editar existente
        isNewItem = false;
        // Asignamos puntero inteligente temporalmente copiando del vector
        // Nota: en una app real podriamos usar weak_ptr o similar, aqui asumimos que el vector no cambia mientras editamos
        currentEditingItem = _monedas[index]; 
        
        txtNombre()->setValue(item->getNombre());
        txtSiglas()->setValue(item->getSiglas());
        txtSimbolo()->setValue(item->getSimbolo());
    } else {
        // Nuevo item
        isNewItem = true;
        txtNombre()->setValue("");
        txtSiglas()->setValue("");
        txtSimbolo()->setValue("");
    }
  };

  _gridMonedas->setOnRowActivated([this, startEdit](int index) { startEdit(index); });
  _gridMonedas->setOnAddRequested([this, startEdit]() { startEdit(-1); });
  
  // Lógica Guardar
  cmdAceptar()->setOnExecuted([this]() {
      std::string nombre = txtNombre()->getValue();
      std::string siglas = txtSiglas()->getValue();
      std::string simbolo = txtSimbolo()->getValue();
      
      // Validación básica
      if (nombre.empty() || siglas.empty()) {
          // Podriamos mostrar alerta aqui
          return;
      }
      
      if (isNewItem) {
          int newId = _monedas.empty() ? 1 : _monedas.back()->getId() + 1;
          auto newItem = std::make_shared<Finexa::Moneda>(newId, siglas, nombre, simbolo);
          _monedas.push_back(newItem);
      } else {
          // Actualización de item existente
          // Como 'Moneda' es inmutable en getters (solo tiene getters), y el constructor inicializa todo,
          // necesitariamos setters o recrear el objeto.
          // Dado que el original Moneda.h define miembros privados y solo getters, y un constructor, 
          // asumiremos que debemos reemplazar el objeto en el vector o que Moneda deberia tener setters.
          // Por simplicidad y dado el diseño inmutable aparente, reemplazaremos el puntero en el vector.
          
          if (currentEditingItem) {
              // Buscar índice de currentEditingItem
               for(size_t i=0; i<_monedas.size(); ++i) {
                   if (_monedas[i]->getId() == currentEditingItem->getId()) {
                       _monedas[i] = std::make_shared<Finexa::Moneda>(currentEditingItem->getId(), siglas, nombre, simbolo);
                       break;
                   }
               }
          }
      }
      
      _gridMonedas->setIsInputLocked(false);
      currentEditingItem.reset();
      refreshRows();
  });
  
  // Lógica Cancelar
  cmdCancelar()->setOnExecuted([this]() {
      _gridMonedas->setIsInputLocked(false);
      currentEditingItem.reset();
  });
}

void NOMBRE_VIEW_MODEL::refreshRows() {
  std::vector<DcGridRow> rows;
  
  for (const auto& moneda : _monedas) {
    DcGridRow r;
    // Tag para identificar el objeto (podemos usar raw pointer del objeto gestionado)
    r.tag = moneda.get(); 
    
    r.cells.push_back(moneda->getSiglas());
    r.cells.push_back(moneda->getNombre());
    r.cells.push_back(moneda->getSimbolo());
    r.cells.push_back(""); // Placeholder para acciones
    
    rows.push_back(r);
  }
  
  // Fila fantasma para crear nuevos registros si se desea visualmente, 
  // aunque 'OnAddRequested' suele invocarse por botón externo o fila especial.
  // El GridTestViewModel añade una fila fantasma explicita.
  rows.push_back(DcGridRow::CreateGhost(4)); // 4 columnas
  
  _gridMonedas->setRows(rows);
}

} // namespace MiApp::ViewModels

// =========================================================
// C-Bridge Function Implementations
// =========================================================

IMPLEMENT_VIEWMODEL_LIFECYCLE(MiApp::ViewModels, NOMBRE_VIEW_MODEL)

IMPLEMENT_CONTROL_BRIDGE(MiApp::ViewModels, NOMBRE_VIEW_MODEL, GRD_MONEDAS);
IMPLEMENT_CONTROL_BRIDGE(MiApp::ViewModels, NOMBRE_VIEW_MODEL, TXT_NOMBRE);
IMPLEMENT_CONTROL_BRIDGE(MiApp::ViewModels, NOMBRE_VIEW_MODEL, TXT_SIGLAS);
IMPLEMENT_CONTROL_BRIDGE(MiApp::ViewModels, NOMBRE_VIEW_MODEL, TXT_SIMBOLO);
IMPLEMENT_CONTROL_BRIDGE(MiApp::ViewModels, NOMBRE_VIEW_MODEL, CMD_ACEPTAR);
IMPLEMENT_CONTROL_BRIDGE(MiApp::ViewModels, NOMBRE_VIEW_MODEL, CMD_CANCELAR);
