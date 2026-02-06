// Las líneas o bloques de comentario que comienzan con <ref> son solo para
// explicar el uso de este template y no deben ser generadas en el código final.
/**
 * @file Nombre del archivo (generalmente el mismo que el nombre de la
 * clase.cpp).
 * @date AAAA-MM-DD
 * @brief Resumen del archivo.
 */

#include "NombreClase.hpp"

namespace NAMESPACE {

// =========================================================
// Ciclo de vida
// =========================================================

std::shared_ptr<NOMBRE_VIEW_MODEL> NOMBRE_VIEW_MODEL::create() {
  return std::make_shared<NOMBRE_VIEW_MODEL>();
}

NOMBRE_VIEW_MODEL::NOMBRE_VIEW_MODEL() {

  // <ref>Creación de view models de controles utilizando el factory en DcVmFactory.hpp
  _cbxBeneficiario = createComboBoxViewModelPtr();
  _dtpFecha = createDateViewModelPtr();
  _txtDescripcion = createInputViewModelPtr();
  _cmdAceptar = createCommandViewModelPtr();
  _cmdCancelar = createCommandViewModelPtr();
  _txtMonto = createNumberFieldViewModelPtr();
  _chkActivo = createCheckBoxViewModelPtr();

  // <ref>Resto de las funciones que deban ser llamadas en el constructor
  // <ref>Ejemplo:
  setUpControls();
}

// =========================================================
// Definiciones de accesores de bindings
// =========================================================

/*<ref> La macro IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, NOMBRE_VIEW_MODEL,
MACRO_CONTROL, TIPO_CONTROL) se encarga de crear el accesor del view model del
control. CONCAT_BINDING(TIPO_CONTROL) *DC_FULL_CLASS(NAMESPACE,
NOMBRE_VIEW_MODEL)::DC_CONCAT(MACRO_CONTROL,Binding)() { return
static_cast<CONCAT_BINDING(TIPO_CONTROL) *>(DC_CONCAT(_, MACRO_CONTROL).get());
}
CONCAT_IVIEWMODEL(TIPO_CONTROL) *DC_FULL_CLASS(NAMESPACE,
NOMBRE_VIEW_MODEL)::MACRO_CONTROL() { return DC_CONCAT(_, MACRO_CONTROL).get();
}
Ejemplo: IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, NOMBRE_VIEW_MODEL, CMD_ACEPTAR,
Command) genera:
ICommandBinding* NAMESPACE::NOMBRE_VIEW_MODEL::cmdAceptarBinding() {
    return static_cast<ICommandBinding *>_cmdAceptar.get();
}
ICommandViewModel* NAMESPACE::NOMBRE_VIEW_MODEL::cmdAceptar() {
    return _cmdAceptar.get();
}
*/
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, NOMBRE_VIEW_MODEL, CMD_ACEPTAR, Command);
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, NOMBRE_VIEW_MODEL, CMD_CANCELAR, Command);
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, NOMBRE_VIEW_MODEL, TXT_DESCRIPCION,
                           Input);
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, NOMBRE_VIEW_MODEL, CBX_BENEFICIARIO,
                           ComboBox);
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, NOMBRE_VIEW_MODEL, DTP_FECHA, DatePicker);
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, NOMBRE_VIEW_MODEL, TXT_MONTO,
                           NumberField);
IMPLEMENT_CONTROL_INTERNAL(NAMESPACE, NOMBRE_VIEW_MODEL, CHK_ACTIVO, CheckBox);

// =========================================================
// Funciones privadas de utileria
// =========================================================

void NOMBRE_VIEW_MODEL::setUpControls() {
  // <ref>Configuración de view models de controles
  cmdAceptar()->setLabelText("Aceptar");
  cmdAceptar()->setOnExecuted([this]() {
    // <ref>Instrucciones para el evento OnExecuted del control
  });

  cmdCancelar()->setLabelText("Cancelar");
  cmdCancelar()->setOnExecuted([this]() {
    // <ref>Instrucciones para el evento OnExecuted del control
  });

  txtDescripcion()->setLabelText("Descripción");
  txtDescripcion()->setOnTextChanged([this](const std::string &text) {
    // <ref>Instrucciones para el evento OnTextChanged del control
  });

  cbxBeneficiario()->setLabelText("Beneficiario");
  cbxBeneficiario()->setOnSelectionChanged([this](const std::string &value) {
    // <ref>Instrucciones para el evento OnSelectionChanged del control
  });

  dtpFecha()->setLabelText("Fecha");
  dtpFecha()->setOnDateChanged([this](const std::string &date) {
    // <ref>Instrucciones para el evento OnDateChanged del control
  });

  txtMonto()->setLabelText("Monto");
  txtMonto()->setOnValueChanged([this](const std::string &value) {
    // <ref>Instrucciones para el evento OnValueChanged del control
  });

  chkActivo()->setLabelText("Activo");
  chkActivo()->setOnCheckedChanged([this](bool isChecked) {
    // <ref>Instrucciones para el evento OnCheckedChanged del control
  });
}

// =========================================================
// Funciones públicas
// =========================================================

bool NOMBRE_VIEW_MODEL::ejemploFuncion(int parametro1, std::string parametro2) {
  // <ref>Instrucciones de la función pública
  return true;
}

} // namespace NAMESPACE

// =========================================================
// C-Bridge Function Implementations
// =========================================================

IMPLEMENT_VIEWMODEL_LIFECYCLE(NAMESPACE, NOMBRE_VIEW_MODEL)

IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, NOMBRE_VIEW_MODEL, CMD_ACEPTAR);
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, NOMBRE_VIEW_MODEL, CMD_CANCELAR);
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, NOMBRE_VIEW_MODEL, TXT_DESCRIPCION);
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, NOMBRE_VIEW_MODEL, CBX_BENEFICIARIO);
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, NOMBRE_VIEW_MODEL, DTP_FECHA);
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, NOMBRE_VIEW_MODEL, TXT_MONTO);
IMPLEMENT_CONTROL_BRIDGE(NAMESPACE, NOMBRE_VIEW_MODEL, CHK_ACTIVO);