Actua como un desarrollador de software con experiencia en Swift y c++.
Tu objetuvo es crear los controlers para la sección de operaciones.

La sección consta de una grilla para visualizar registros de operaciones, y la posibilidad de agregar, editar y eliminar registros.
En la parte de arriba de la grilla, se debe mostrar la moneda refencial en la que se están haciendo los calculos
En la parte de abajo, se refleja el monto total en moneda refencial de todas las operaciones.

La vista debe contener la grilla con los registros de operaciones y un botón de agregar operación en la barra de navegación.

Al hacer click en el botón de agregar operación, debe presentarse el formulario para agregar una nueva operación.
Al hacer click en una fila, se debe mostrar el formulario para editar la operación.
Al hacer swipe en una fila, se debe mostrar el botón de eliminar.
Si se le da al botón de eliminar, se debe eliminar el registro y actualizar la grilla.


El editor debe contener los campos:
- Descripcion
- Moneda
- Monto
- MontoEnMonedaRef

La barra de comando (adjunta al tope del input view) debe contener los botones:
- Tab Prev
- Aceptar (Crear cuando es nuevo, actualizar cuando es edicion)
- Cancelar
- Tab Next

En la version ios, la grilla se crea en el archivo OperacionesController.swift, y el editor se crea en el archivo EditorOperacionController.swift

para la grilla, debes usar el estilo visual de layout del archivo operaciones.png
puedes usar el mismo estilo visual del EditorMonedaController.swift

Para el binder, el view model es el OperacionesViewModel (Finexa/Common/ViewModels/include/OperacionesViewModel.h y Finexa/Common/ViewModels/src/OperacionesViewModel.cpp)


## Tareas
Genera los archivos
- Finexa/iOS/Views/Controllers/OperacionesController.swift
- Finexa/iOS/Views/Controllers/EditorOperacionController.swift
- Finexa/iOS/Views/BindingsInterfaces/OperacionesBinder.swift
- Finexa/iOS/Views/BindingsInterfaces/EditorOperacionBinder.swift
- Finexa/ios/App/Source/OperacionesAppBinder.swift
- Finexa/ios/App/Source/EditorOperacionesAppBinder.swift

- Actualiza el App Delegate para que comience con el grid de las operaciones en lugar de las monedas para probarlo.