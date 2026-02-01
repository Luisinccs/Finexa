# Requerimientos: Calculadora Financiera Finexa

## 1. Objetivo del Sistema
Desarrollar una herramienta financiera que permita determinar de manera rápida y precisa un monto total expresado en una moneda base (**XDS**), gestionando dinámicamente tasas de cambio y operaciones.

---

## 2. Definición de la Interfaz de Usuario

### 2.1. Configuración de Tasas
El sistema debe permitir visualizar y establecer la relación de valor entre pares de monedas.
*Ejemplo de configuración:*
*   **VES / EUR:** 380,00
*   **VES / USD:** 330,00
*   **VES / XDS:** 550,00

### 2.2. Vista Principal (Operaciones)
Grilla donde se registran los movimientos financieros. El sistema calcula automáticamente el equivalente en la moneda objetivo.

| Concepto | Monto Original | Moneda | Monto XDS (Calculado) |
| :--- | :--- | :--- | :--- |
| Mercado | 100 | USD | 60,00 |
| Servicio | 20 | EUR | 13,82 |
| Combustible | 20 | USD | 12,00 |
| Pescadero | 10.000 | VES | 18,18 |
| **TOTAL** | | | **104,00** |

---

## 3. Lógica de Negocio y Reglas de Conversión

El desafío técnico reside en calcular tasas derivadas evitando ambigüedades.
*   **Caso de Conflicto:** Si existe una tasa `VES/USD` y una `VES/XDS`, establecer una tasa manual `XDS/USD` crea una ambigüedad si no coincide con el cálculo implícito.
*   **Interfaz:** Debe ser sencilla, amigable y enfocada en la rapidez de entrada de datos.

---

## 4. Requerimientos Funcionales

### RF-01: Gestión Dinámica de Monedas
El sistema debe permitir el registro de monedas **"on-the-fly"**.
*   **Entidades:** `ID` (int), `Nombre` (string), `Siglas` (ISO), `Símbolo` (string).
*   **Comportamiento:** Si el usuario ingresa una sigla no registrada en un ComboBox de tasas u operaciones, el sistema solicitará confirmación para crearla en el acto.

### RF-02: Configuración de Tasas de Cambio
Interfaz dedicada a la gestión de pares de divisas.
*   **Entrada:** Moneda 1, Moneda 2, Valor numérico.
*   **Edición:** Solo se permite modificar el valor numérico. Para cambiar las monedas de una tasa, se debe eliminar el registro y crear uno nuevo.

### RF-03: Lógica de Triangulación y Ambigüedad
Reglas para garantizar la integridad de los cálculos:
1.  **Moneda Pivote:** Se define `VES` como la moneda pivote global para las triangulaciones.
2.  **Cálculo Implícito:** Al intentar ingresar una tasa directa (Ej: `USD/EUR`), el sistema calculará un valor sugerido basado en las tasas existentes contra el pivote (`VES`).
3.  **Alerta de Ambigüedad:** Si existe una diferencia mayor a **0.001** entre el valor manual ingresado y el valor calculado implícitamente, el sistema debe disparar una alerta y solicitar al usuario cuál valor debe prevalecer.

### RF-04: Tabla de Operaciones Financieras
Grilla principal de registro de movimientos.
*   **Campos Requeridos:** Concepto, Monto Original, Moneda Original, Monto XDS (Calculado).
*   **Totales:** Sumatoria dinámica en tiempo real del total acumulado en la moneda objetivo (**XDS**).