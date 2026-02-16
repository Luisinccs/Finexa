
Actua como un desarrollador de software con experiencia en Swift y c++.
Tu objetuvo es crear el view model y los controlers para la sección de monedas.

La sección consta de una grilla para visualizar registros de monedas y la posibilidad de agregar, editar y eliminar registros.

## Vistas 

### iOS

La vista debe contener la grilla con los registros de monedas y un botón de agregar moneda en la barra de navegación.
Al hacer click en el botón de agregar moneda, debe presentarse el formulario para agregar una nueva moneda.
Al hacer swipe en una filas, se deben mostrar las opciones de editar y eliminar.
Al hacer click en el botón de editar, debe presentarse el formulario para editar la moneda.
Al hacer click en el botón de eliminar, debe eliminar la moneda y actualizar la grilla.

Las filas deben mostrar los registros de las monedas de la forma:

| Nombre (Simbolo) ------- Siglas |
Ejemplo
| Dolar ($) ---------------- USD |

El editor debe contener los campos:
- Nombre
- Simbolo
- Siglas

La barra de comando (adjunta al tope del input view) debe contener los botones:
- Tab Prev
- Aceptar (Crear cuando es nuevo, actualizar cuando es edicion)
- Cancelar
- Tab Next

En la version ios, la grilla se crea en el archivo MonedasController.swift, y el editor se crea en el archivo EditorMonedaController.swift

Guiate por el archivo DcViews/iOS/Tests/Tests/DcGridTestController.swift para la grilla y por el archivo DcViews/iOS/Tests/Tests/DcInputTestController.swift para el editor.

Utiliza el estilo visual para el layout y la tipografía por el archivo Monedas ios.png. Utiliza colores similares 

### Mac

Se debe mostrar la grilla con los mismos campos, solo que se muestra tambien el encabezado.
En lugar de presentar el formulario de edición  (para cerar o modificar), las filas de la grilla son del patrón "in situ"; Es decir, la fila pasa a ser "editable", presentandose los respectivos campos para editar el registro, y los respectivos botones al final de
- Aceptar (crear cuando es nuevo, actualizar cuando es edicion)
- Cancelar
- eliminar

En la versión mac, todo se crea en el mismo archicvo MonedasController.swift

* Guiate por el archivo DcViews/Mac/Tests/Tests/DcGridTestController.swift.
* Utiliza el estilo visual para el layout y la tipografía por el archivo Grilla monedas mac.png. Utiliza colores similares 

---

## ViewModels

Debe contener los ViewModels:
- DcGridViewModel _monedasGridViewModel; 
- DcInputViewModel _nombreViewModel;
- DcInputViewModel _simboloViewModel;
- DcInputViewModel _siglasViewModel;
- DcCommandViewModel _aceptarViewModel;
- DcCommandViewModel _cancelarViewModel;
- DcCommandViewModel _eliminarViewModel;

- De momento, el view model contiene la data de las monedas en un vector de Monedas. Mas adelante se hará un refactor para llevarlo a una capa de negocio que se conecte con la base de datos.

En el mismo archivo, puedes colocar la estructura de la moneda; luego la llevaremos a una capa de negocio.

* Guiate por los archivos FormTemplate.hpp y FormTemplate.cpp para ver como se implementa la estructura de los formularios (en Templates/ViewModels).

## Tareas de generación:
- Archivo MonedasViewModel.hpp en la carpeta DcViews/CommonTestHelpers/include
- Archivo MonedasViewModel.cpp en la carpeta DcViews/CommonTestHelpers/src
- Archivo MonedasController.swift (version ios) en la carpeta DcViews/iOS/Tests/Test, con la grilla.
- Archivo EditorMonedaController.swift (version ios) en la carpeta DcViews/iOS/Tests/Test, con el editor.
- Archivo MonedasController.swift (version Mac) en la carpeta DcViews/Mac/Tests/Test, con la grilla. (Nota que este controlador posee la grilla editable)
- Actualiza el AppShield de ios para que tenga la sección de monedas, actualiza tambien de una vez ese app shield, para que el botón de menú quede a la izquierda y en el lado derecho se pueda colocar el botón de + para crear un nuevo registro en los casos donde aplique.
- Actualiza el AppShield de mac para que tenga la sección de monedas.



El archivo debe contener una grilla para mostrar los registros de monedas con los campos:  
- Nombre
- Simbolo
- Siglas

## Correcciones
- Actualizar los bridging headers


# Errores
- Tengo estos errores de compilación en EditorMonedaController.swift, verifica que el nombre que expone el view model sea el mismo
/Users/luis_sonoma/Desarrollo/DualComponents/DcViews/iOS/Tests/Tests/EditorMonedaController.swift:47:22 Cannot find 'MonedasViewModel_nombreViewModel' in scope
/Users/luis_sonoma/Desarrollo/DualComponents/DcViews/iOS/Tests/Tests/EditorMonedaController.swift:51:22 Cannot find 'MonedasViewModel_simboloViewModel' in scope
/Users/luis_sonoma/Desarrollo/DualComponents/DcViews/iOS/Tests/Tests/EditorMonedaController.swift:55:22 Cannot find 'MonedasViewModel_siglasViewModel' in scope

---
Tengo estos errores:
- /Users/luis_sonoma/Desarrollo/DualComponents/DcViews/iOS/Tests/Tests/MonedasController.swift:7:58 'MonedasViewModelPtr' is not a member type of enum '__ObjC.DualComponents.TestHelpers'
- /Users/luis_sonoma/Desarrollo/DualComponents/DcViews/iOS/Tests/Tests/MonedasController.swift:32:51 Type 'DualComponents.TestHelpers' has no member 'MonedasViewModel'
- /Users/luis_sonoma/Desarrollo/DualComponents/DcViews/iOS/Tests/Tests/MonedasController.swift:41:30 Cannot find 'MonedasViewModel_monedasGridViewModel' in scope
- /Users/luis_sonoma/Desarrollo/DualComponents/DcViews/iOS/Tests/Tests/MonedasController.swift:94:31 Cannot find 'MonedasViewModel_monedasGridViewModel' in scope
- /Users/luis_sonoma/Desarrollo/DualComponents/DcViews/iOS/Tests/Tests/MonedasController.swift:95:18 Cannot find 'DcGrid_RequestAdd' in scope
- /Users/luis_sonoma/Desarrollo/DualComponents/DcViews/iOS/Tests/Tests/MonedasController.swift:122:30 Cannot find 'MonedasViewModel_monedasGridViewModel' in scope
- /Users/luis_sonoma/Desarrollo/DualComponents/DcViews/iOS/Tests/Tests/MonedasController.swift:140:30 Cannot find 'MonedasViewModel_monedasGridViewModel' in scope
- /Users/luis_sonoma/Desarrollo/DualComponents/DcViews/iOS/Tests/Tests/MonedasController.swift:214:31 Cannot find 'MonedasViewModel_monedasGridViewModel' in scope
- /Users/luis_sonoma/Desarrollo/DualComponents/DcViews/iOS/Tests/Tests/MonedasController.swift:224:30 Cannot find 'MonedasViewModel_eliminarViewModel' in scope

--- Tengo un error en tiempo de ejecución ahora cuando pruebo del formualrio de combobox (DcComboBoxTestController.swift); adjunto te envío el capture. Ese error, antes (Lo único que modificaste hoy en el DcViewModels fue el DcViewModelToolkit.hpp, lo demás fue agregar las nuevas pruebas y actualizar los appshields)

---

Bien, vamos a pulir algunos de detalles de la UI en ios.
- La fila fantasma para nuevo registro no debe verse en ios, solo en mac. En ios se usa el botón +.
- Las filas deben tener color de fondo alterno.
- Me parece bien que al tocar la fila se abra el editor de una ves, en ese caso, retira la acción de edición del swipe y deja solo la acción de eliminar.

---

Trata de ajustarte al estilo visual del formuario para editar monedas en ios (Adjunto); El campo que tiene el enfoque, debe verse con un halo para distinguirlo, y los símbolos de tab y tab next deben mostrarse como símbolos (Especificame si esos íconos tengo que buscarlos e incluirlos en los assetes para que funciones).
Otra cosa, al crear un nuevo registro, se debe actualizar la grilla mostrando el registro creado, ahora no se muestra. 