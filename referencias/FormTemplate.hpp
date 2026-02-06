// Las líneas o bloques de comentario que comienzan con <ref> son solo para
// explicar el uso de este template y no deben ser generadas en el código final.
/**
 * @file Nombre del archivo (generalmente el mismo que el nombre de la
 * clase.hpp).
 * @date AAAA-MM-DD
 * @brief Resumen del archivo.
 */

#pragma once

// <ref> Constante con el nombre de la clase
#define NOMBRE_VIEW_MODEL NombreViewModel
// <ref> Constantes con los nombres de los elementos, pueden ser comandos,
// labels, etc.
#define TXT_DESCRIPCION txtDescripcion
#define CBX_BENEFICIARIO cbxBeneficiario
#define DTP_FECHA dtpFecha
#define TXT_MONTO txtMonto
#define CHK_ACTIVO chkActivo
#define CMD_ACEPTAR cmdAceptar
#define CMD_CANCELAR cmdCancelar

// <ref> Includes de frameworks
#include <functional>
#include <memory>
#include <string>
#include <vector>

// <ref> El DcViewModelToolkit.hpp es necesario para que funcionen los macros
// donde se declaran y definen los view models
#include "DcViewModelToolkit.hpp"
// <ref> Header con el factory para crear view models y los includes de cada uno para simplificar el código
#include "DcVmFactory.hpp"
// <ref> Los bindings
#include "IDcBindings.hpp"

// Conveniente para no tener que escribir DualComponents::ViewModels::
using namespace DualComponents::ViewModels;

namespace MiApp::ViewModels {

/**
 * @brief Descripción del View Model.
 */
class NOMBRE_VIEW_MODEL {
  /*<ref>
La Macro DECLARE_CONTROL_BINDING(MACRO_CONTROL, TIPO_CONTROL) se encarga de
declarar la variable privada del view model del control: private:
   std::shared_ptr<Dc[TIPO_CONTROL]ViewModel> _[MACRO_CONTROL]; // Variable
   privada del view model del control. Se usa unicamente para retener el
   objeto; no se debe usar para nada mas, para acceder internamente se debe
   usar el accesor privado del view model del control, y el binding para
   acceder externamente en la UI. I[TIPO_CONTROL]ViewModel*
   [MACRO_CONTROL](); // Accesor privado del view model del control, se usa
   para manipular la lógica dentro de este view model de consumo.
public:
   I[TIPO_CONTROL]Binding* [MACRO_CONTROL]Binding(); // Accesor público del
   binding del view model del control, se usa para manipular la lógica
   desde la UI.
Ejemplo: DECLARE_CONTROL_BINDING(TXT_DESCRIPCION, Input) genera:
private:
   std::shared_ptr<DcInputViewModel> txtDescripcion;
   IInputViewModel* txtDescripcion();
public:
   IInputBinding* txtDescripcionBinding();
*/
  // =========================================================
  // Declaraciones de view models de controles
  // =========================================================
  // <ref> Estos campos son de demostración, los campos que se deben incluir son los que se dictan en el  prompt  de generación
  DECLARE_CONTROL_BINDING(TXT_DESCRIPCION, Input)
  DECLARE_CONTROL_BINDING(CBX_BENEFICIARIO, ComboBox)
  DECLARE_CONTROL_BINDING(DTP_FECHA, DatePicker)
  DECLARE_CONTROL_BINDING(TXT_MONTO, NumberField)
  DECLARE_CONTROL_BINDING(CHK_ACTIVO, CheckBox)
  DECLARE_CONTROL_BINDING(CMD_ACEPTAR, Command)
  DECLARE_CONTROL_BINDING(CMD_CANCELAR, Command)

private:
  // <ref>Variables y funciones privadas del view model.
  // <ref> Ejemplo de función privada
  void setUpControls();

public:
  // <ref>Constructor
  NOMBRE_VIEW_MODEL();
  // <ref>Destructor
  virtual ~NOMBRE_VIEW_MODEL() = default;
  // <ref>Factory para crear view model
  static std::shared_ptr<NOMBRE_VIEW_MODEL> create();
  // <ref>Resto de las funciones publicas si las hay. Cada función debe tener su
  // comentario con el estilo:
  /**
   * @brief Descripción de la función.
   * @param parametro1 Descripción del parámetro 1.
   * @param parametro2 Descripción del parámetro 2.
   * @return Descripción del valor retornado.
   */
  //<ref>Ejemplo de función pública
  bool ejemploFuncion(int parametro1, std::string parametro2);
};

// <ref>Alias para el puntero inteligente del view model
using NOMBRE_VIEW_MODEL_PTR = std::shared_ptr<NOMBRE_VIEW_MODEL>;

} // namespace MiApp::ViewModels

// <ref>Funciones C-Bridge para Swift/C#/JavaScript/Kotlin
// =========================================================
// C-Bridge Functions (Para Swift/C#/JavaScript/Kotlin)
// =========================================================

/**
 <ref> La macro DECLARE_VIEWMODEL_LIFECYCLE(ClassName) se encarga de Declarar
 las funciones de ciclo de vida (Create y Destroy) para el C-Bridge.
 * Genera:
 *   DC_BRIDGE_EXPORT void* ClassName_Create();
 *   DC_BRIDGE_EXPORT void ClassName_Destroy(void* vmPtr);
 */
DECLARE_VIEWMODEL_LIFECYCLE(NOMBRE_VIEW_MODEL)

/**
 <ref> La macro DECLARE_CONTROL_BRIDGE(MACRO_CONTROL, ClassName) se encarga de
 declarar las funciones de bridge para el control.
 * Genera:
 *   DC_BRIDGE_EXPORT void* ClassName_MACRO_CONTROL(void* vmPtr);
 */
DECLARE_CONTROL_BRIDGE(TXT_DESCRIPCION, NOMBRE_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(CBX_BENEFICIARIO, NOMBRE_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(DTP_FECHA, NOMBRE_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(TXT_MONTO, NOMBRE_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(CHK_ACTIVO, NOMBRE_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(CMD_ACEPTAR, NOMBRE_VIEW_MODEL)
DECLARE_CONTROL_BRIDGE(CMD_CANCELAR, NOMBRE_VIEW_MODEL)

//<ref>Funciones C-Bridge para funciones públicas
DC_BRIDGE_EXPORT bool NOMBRE_VIEW_MODEL_ejemploFuncion(void *vmPtr,
                                                       int parametro1,
                                                       std::string parametro2);