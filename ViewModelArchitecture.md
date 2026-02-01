# Guía de Generación de ViewModels (DualComponents Architecture)

Esta guía define el patrón de arquitectura para generar ViewModels de consumo compatibles con el sistema de bindings multiplataforma (C-Bridge) de DualComponents.

## 1. Patrón de Macros (DcViewModelToolkit.hpp)

El núcleo de la arquitectura se basa en cinco macros principales:

1.  **`DECLARE_CONTROL_BINDING(nombre, Tipo)`**: Se usa en el `.hpp` dentro de la clase. Declara el `shared_ptr` privado, el accesor del ViewModel y el accesor del Binding.
2.  **`DECLARE_CONTROL_BRIDGE(nombre, Clase)`**: Se usa en el `.hpp` (fuera de la clase). Declara la función exportada para el puente C.
3.  **`IMPLEMENT_CONTROL_INTERNAL(Namespace, Clase, nombre, Tipo)`**: Se usa en el `.cpp`. Implementa los métodos de acceso internos.
4.  **`IMPLEMENT_VIEWMODEL_LIFECYCLE(Namespace, Clase)`**: Se usa en el `.cpp`. Implementa las funciones `_Create` y `_Destroy` para el puente C.
5.  **`IMPLEMENT_CONTROL_BRIDGE(Namespace, Clase, nombre)`**: Se usa en el `.cpp`. Implementa la exportación del binding para el puente C.

## 2. Estructura Sugerida del Archivo Header (`.hpp`)

```cpp
#pragma once

// 1. Definiciones de conveniencia
#define MY_VM_NAME MyCustomViewModel
#define MY_CONTROL_1 myButton

#include "DcViewModelToolkit.hpp"
#include "Dc[Tipo]ViewModel.hpp"
#include "I[Tipo]Binding.hpp"

namespace DualComponents::MyNamespace {

class MyCustomViewModel {
    // 2. Declaración de bindings
    DECLARE_CONTROL_BINDING(MY_CONTROL_1, [Tipo]) 

public:
    MyCustomViewModel();
    virtual ~MyCustomViewModel() = default;

    static std::shared_ptr<MyCustomViewModel> create();
};

} // namespace

// 3. Declaraciones para el C-Bridge (Fuera del namespace)
DECLARE_VIEWMODEL_LIFECYCLE(MY_VM_NAME)
DECLARE_CONTROL_BRIDGE(MY_CONTROL_1, MY_VM_NAME)
```

## 3. Estructura Sugerida del Archivo de Implementación (`.cpp`)

```cpp
#include "MyCustomViewModel.hpp"

namespace DualComponents::MyNamespace {

// 1. Factory method
std::shared_ptr<MyCustomViewModel> MyCustomViewModel::create() {
    return std::make_shared<MyCustomViewModel>();
}

// 2. Constructor: Inicialización de controles
MyCustomViewModel::MyCustomViewModel() {
    _myButton = std::make_shared<Dc[Tipo]ViewModel>();
}

// 3. Implementación de accesores internos
IMPLEMENT_CONTROL_INTERNAL(DualComponents::MyNamespace, MY_VM_NAME, MY_CONTROL_1, [Tipo])

} // namespace

// 4. Implementación del C-Bridge
IMPLEMENT_VIEWMODEL_LIFECYCLE(DualComponents::MyNamespace, MY_VM_NAME)
IMPLEMENT_CONTROL_BRIDGE(DualComponents::MyNamespace, MY_VM_NAME, MY_CONTROL_1)
```

## 4. Mapeo de Tipos de Control

Al usar las macros, el `Tipo` (segundo argumento en `DECLARE_CONTROL_BINDING` y cuarto en `IMPLEMENT_CONTROL_INTERNAL`) debe coincidir con el prefijo de las clases base:

| Tipo | Clase Interna | Interfaz VM | Interfaz Binding |
| :--- | :--- | :--- | :--- |
| **Command** | `DcCommandViewModel` | `ICommandViewModel` | `ICommandBinding` |
| **Input** | `DcInputViewModel` | `IInputViewModel` | `IInputBinding` |
| **Check** | `DcCheckViewModel` | `ICheckViewModel` | `ICheckBinding` |
| **ComboBox** | `DcComboBoxViewModel` | `IComboBoxViewModel` | `IComboBoxBinding` |
| **Grid** | `DcGridViewModel` | `IGridViewModel` | `IGridBinding` |
| **NumberField** | `DcNumberFieldViewModel` | `INumberFieldViewModel` | `INumberFieldBinding` |
| **DatePicker** | `DcDatePickerViewModel` | `IDatePickerViewModel` | `IDatePickerBinding` |

## 5. Reglas Importantes

*   **Nombres de variables**: La macro `DECLARE_CONTROL_BINDING(name, ...)` crea una variable privada llamada `_name` (con guion bajo).
*   **Convención de NAMESPACE**: Si usas una macro `#define NAMESPACE ...` en un archivo común, puedes usarla en los `IMPLEMENT_...` para evitar repeticiones.
*   **Exportación**: Asegúrate de que `DC_BRIDGE_EXPORT` esté correctamente definido (generalmente incluido vía `DcViewModelToolkit.hpp`).
