Actua como un desarrollador de software con experiencia en Swift y c++.
Tu objetuvo es crear los controlers para la sección de tasas de cambio.

La sección consta de una grilla para visualizar registros de tasas de cambio y la posibilidad de agregar, editar y eliminar registros.

La vista debe contener la grilla con los registros de tasas de cambio y un botón de agregar tasa en la barra de navegación.

Al hacer click en el botón de agregar tasa, debe presentarse el formulario para agregar una nueva tasa.
Al hacer click en una fila, se debe mostrar el formulario para editar la tasa.
Al hacer swipe en una fila, se debe mostrar el botón de eliminar.
Si se le da al botón de eliminar, se debe eliminar el registro y actualizar la grilla.

Las filas deben mostrar los registros de las tasas de cambio de la forma:

| Moneda1 X Moneda2 ------- Tasa |
Ejemplo
| Dolar ($) X Euro (€) ------- 1.00 |

El editor debe contener los campos:
- Moneda1
- Moneda2
- Tasa

La barra de comando (adjunta al tope del input view) debe contener los botones:
- Tab Prev
- Aceptar (Crear cuando es nuevo, actualizar cuando es edicion)
- Cancelar
- Tab Next

En la version ios, la grilla se crea en el archivo TasasController.swift, y el editor se crea en el archivo EditorTasaController.swift

puedes usar el mismo estilo visual de MonedasController.swift y EditorMonedaController.swift

Para el binder, el view model es el TasasViewModel (Finexa/Common/ViewModels/include/TasasViewModel.h y Finexa/Common/ViewModels/src/TasasViewModel.cpp)


## Tareas
Genera los archivos
- Finexa/iOS/Views/Controllers/TasasController.swift
- Finexa/iOS/Views/Controllers/EditorTasaController.swift
- Finexa/iOS/Views/BindingsInterfaces/TasasBinder.swift
- Finexa/iOS/Views/BindingsInterfaces/EditorTasaBinder.swift
- Finexa/ios/App/Source/TasasAppBinder.swift
- Finexa/ios/App/Source/EditorTasasAppBinder.swift

- Actualiza el App Delegate para que comience con el grid de las tasas en lugar de las monedas para probarlo.

----

Bien, hay que meterle cabeza a esto.
Necesitamos crear un mecanismo para validar las tasas cuando se crean de modo de evitar las triangulaciones.
Es decir, si hay una tasa VES/USD y otra VES/EUR, no se puede crear una tercera tasa USD/EUR porque viola la regla de las triangulaciones.
Nuevamente, la validación se debe realizar en el view model. y se debe mostrar al usuario un mensaje explicando por qué no se puede crear la tasa.