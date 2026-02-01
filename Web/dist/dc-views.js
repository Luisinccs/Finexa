// dc-views.js

/**
 * Control Base para manejo común
 */
class DcControl {
    constructor(vm) {
        this.vm = vm;
        this.element = null;
    }

    mount(parentId) {
        const parent = document.getElementById(parentId);
        if (parent && this.element) {
            parent.appendChild(this.element);
        } else {
            console.error(`No se encontró el contenedor: ${parentId}`);
        }
    }

    dispose() {
        if (this.vm && this.vm.delete) {
            this.vm.delete(); // Limpieza manual de C++
            this.vm = null;
        }
        if (this.element) {
            this.element.remove();
        }
    }
}

/**
 * DcNumberTextField: Input numérico estricto
 */
export class DcNumberTextField extends DcControl {
    constructor(vm) {
        super(vm);
        this.create();
        this.setupBindings();
    }

    create() {
        this.element = document.createElement('input');
        this.element.type = "text";
        this.element.className = "dc-input dc-number";
        this.element.readOnly = true; // Evitar teclado móvil nativo si queremos control total
        // O dejarlo editable y interceptar keydown como en Mac

        // Interceptación de teclado
        this.element.addEventListener('keydown', (e) => {
            e.preventDefault();
            if (e.key >= '0' && e.key <= '9') this.vm.pushDigit(parseInt(e.key));
            if (e.key === 'Backspace') this.vm.deleteDigit();
            if (e.key === '-') this.vm.toggleSign();
        });
    }

    setupBindings() {
        // C++ -> JS
        this.vm.bindTextChange((text) => {
            this.element.value = text;
        });
        // Inicial
        this.element.value = this.vm.getFormattedText();
    }
}

/**
 * DcInput: Input de texto normal
 */
export class DcInput extends DcControl {
    constructor(vm) {
        super(vm);
        this.create();
        this.setupBindings();
    }

    create() {
        this.element = document.createElement('input');
        this.element.type = "text";
        this.element.className = "dc-input";

        this.element.addEventListener('input', (e) => {
            this.vm.onUserInput(e.target.value);
        });
    }

    setupBindings() {
        this.vm.bindTextChange((text) => {
            if (this.element.value !== text) {
                this.element.value = text;
            }
        });
        this.element.value = this.vm.getText();
        this.element.placeholder = this.vm.getPlaceholder();
    }
}

/**
 * DcComboBox Mejorado
 * Soporta navegación por teclado, selección visual (tail) y cierre al hacer clic fuera.
 */
export class DcComboBox extends DcControl {
    constructor(vm) {
        super(vm);
        this.activeIndex = -1; // Para navegación por teclado
        this.create();
        this.setupBindings();
        this.setupGlobalEvents();
    }

    create() {
        this.container = document.createElement('div');
        this.container.className = "dc-combobox-wrapper";

        this.input = document.createElement('input');
        this.input.type = "text";
        this.input.className = "dc-input";

        this.arrow = document.createElement('button');
        this.arrow.className = "dc-combo-arrow";
        this.arrow.innerHTML = "▼";
        this.arrow.tabIndex = -1;

        this.list = document.createElement('ul');
        this.list.className = "dc-combo-list";
        this.list.style.display = 'none';

        // --- Eventos Input ---
        this.input.addEventListener('input', (e) => {
            // Enviamos el texto crudo, C++ decide el autocompletado
            this.vm.onUserInput(e.target.value);
            this.showList(true);
        });

        this.input.addEventListener('keydown', (e) => this.handleKeyDown(e));

        // --- Eventos Flecha ---
        this.arrow.addEventListener('click', (e) => {
            e.stopPropagation(); // Evitar que el click en la flecha cierre la lista inmediatamente
            this.input.focus();
            const isVisible = this.list.style.display === 'block';
            if (!isVisible) {
                // Forzar refresco de lista al abrir
                this.vm.onUserInput(this.input.value);
            }
            this.showList(!isVisible);
        });

        this.container.appendChild(this.input);
        this.container.appendChild(this.arrow);
        this.container.appendChild(this.list);
        this.element = this.container;
    }

    setupBindings() {
        // 1. Texto y Tail Selection
        this.vm.bindTextChange((text) => {
            this.input.value = text;

            // Lógica de Selección Azul (Igual que en Mac)
            const start = this.vm.getSelectionStart();
            if (start < text.length) {
                // Seleccionar la "cola" sugerida
                this.input.setSelectionRange(start, text.length);
            }
        });

        // 2. Actualización de la Lista
        this.vm.bindListUpdate(() => {
            this.activeIndex = -1; // Resetear navegación
            this.renderList();
        });

        this.input.placeholder = this.vm.getPlaceholder();
        this.renderList();
    }

    // --- Manejo de Teclado (Flechas y Enter) ---
    handleKeyDown(e) {
        const count = this.list.children.length;

        // [NUEVO] Interceptar Backspace para evitar el "Yo-Yo"
        if (e.key === 'Backspace') {
            e.preventDefault(); // 1. Detener borrado nativo del navegador
            this.vm.onBackspace(); // 2. Delegar lógica inteligente al Core C++
            this.showList(true); // 3. Asegurar que la lista siga visible al borrar
            return;
        }

        if (e.key === 'ArrowDown') {
            e.preventDefault();
            this.showList(true);
            if (this.activeIndex < count - 1) {
                this.activeIndex++;
                this.highlightItem(this.activeIndex);
            }
        }
        else if (e.key === 'ArrowUp') {
            e.preventDefault();
            if (this.activeIndex > 0) {
                this.activeIndex--;
                this.highlightItem(this.activeIndex);
            }
        }
        else if (e.key === 'Enter' || e.key === 'Tab') {
            if (this.list.style.display === 'block' && this.activeIndex >= 0) {
                // Si hay un ítem resaltado en la lista, lo seleccionamos
                this.vm.onSelection(this.activeIndex);
                e.preventDefault(); // Evitar submit del form si lo hubiera
            } else {
                // Si no, confirmamos lo que hay en el texto
                this.vm.confirmSelection();
            }
            this.showList(false);
        }
        else if (e.key === 'Escape') {
            this.showList(false);
            // Opcional: llamar a this.vm.cancel() si lo expusiste
        }
    }

    renderList() {
        this.list.innerHTML = '';
        const count = this.vm.getFilteredItemsCount();

        if (count === 0) {
            this.showList(false);
            return;
        }

        for (let i = 0; i < count; i++) {
            const item = this.vm.getFilteredItem(i);
            const itemText = item.text;
            const li = document.createElement('li');
            li.innerText = itemText;

            // Clic en ítem
            li.onmousedown = () => {
                this.vm.onSelection(i);
                this.showList(false);
            };

            this.list.appendChild(li);
        }
    }

    highlightItem(index) {
        // Remover clase 'active' de todos
        Array.from(this.list.children).forEach(li => li.classList.remove('active'));

        // Agregar a la selección actual
        const item = this.list.children[index];
        if (item) {
            item.classList.add('active');
            item.scrollIntoView({ block: 'nearest' }); // Auto-scroll
        }
    }

    showList(show) {
        this.list.style.display = show ? 'block' : 'none';
    }

    // Cerrar lista si se hace clic fuera
    setupGlobalEvents() {
        document.addEventListener('click', (e) => {
            if (!this.element.contains(e.target)) {
                this.showList(false);
                // Opcional: confirmar selección al perder foco
                // this.vm.confirmSelection(); 
            }
        });
    }
}

/**
 * DcCheck: Checkbox
 */
export class DcCheck extends DcControl {
    constructor(vm) {
        super(vm);
        this.element = document.createElement('div');
        this.element.className = "dc-check-wrapper";

        this.input = document.createElement('input');
        this.input.type = "checkbox";

        this.label = document.createElement('label');

        this.element.appendChild(this.input);
        this.element.appendChild(this.label);

        // Evento UI -> C++
        this.input.addEventListener('change', (e) => {
            this.vm.updateFromUI(e.target.checked);
        });

        // Binding C++ -> UI
        this.vm.bindValueChange((val) => {
            this.input.checked = val;
        });

        this.label.innerText = this.vm.getLabelText();
        this.input.checked = this.vm.getValue();
    }
}

/**
 * DcButton: Botón
 */
export class DcButton extends DcControl {
    constructor(vm) {
        super(vm);
        this.element = document.createElement('button');
        this.element.className = "dc-btn";
        this.element.innerText = this.vm.getLabelText();

        // Usamos mousedown para mejor respuesta
        this.element.addEventListener('click', () => {
            if (this.vm && this.vm.execute) {
                this.vm.execute();
            } else {
                console.error("Method execute() not found on ViewModel. Check Bindings.cpp");
            }
        });
    }
}

/**
 * DcGrid: Grilla de Datos
 */
export class DcGrid extends DcControl {
    constructor(vm) {
        super(vm);
        console.log('DcGrid constructor called, vm:', vm);
        this.create();
        this.setupBindings();
        this.render();
        console.log('DcGrid constructor finished');
    }

    create() {
        console.log('DcGrid.create() called');
        this.element = document.createElement('div');
        this.element.className = "dc-grid-container";
        this.element.tabIndex = 0; // Para recibir foco y teclado

        this.table = document.createElement('table');
        this.table.className = "dc-grid-table";

        this.thead = document.createElement('thead');
        this.tbody = document.createElement('tbody');

        this.table.appendChild(this.thead);
        this.table.appendChild(this.tbody);
        this.element.appendChild(this.table);

        // Eventos
        this.element.addEventListener('keydown', (e) => this.handleKeyDown(e));
        console.log('DcGrid.create() finished, element:', this.element);
    }

    setupBindings() {
        console.log('DcGrid.setupBindings() called');
        // Datos cambiaron
        this.vm.bindDataChanged(() => {
            console.log("Grid Data Changed");
            this.render();
        });

        // Selección cambió desde C++
        this.vm.bindSelectionChanged((index) => {
            this.updateSelectionVisuals(index);
        });

        // Input Locked (Bloqueo de UI durante edición)
        this.vm.bindInputLockedChanged((locked) => {
            this.element.classList.toggle('disabled', locked);
        });
    }

    render() {
        console.log('DcGrid.render() called');
        this.renderHeaders();
        this.renderHeaders();
        this.renderRows();
        this.updateSelectionVisuals(this.vm.getSelectedIndex());
    }

    renderHeaders() {
        this.thead.innerHTML = '';
        const tr = document.createElement('tr');

        // Obtener columnas
        const columns = this.vm.getColumns(); // Vector<DcGridColumn>
        const count = columns.size();

        for (let i = 0; i < count; i++) {
            const col = columns.get(i);
            const th = document.createElement('th');
            th.innerText = col.header;
            // Podríamos usar col.width (como factor flex o porcentaje)
            if (col.width > 0) {
                th.style.width = col.width + "em";
            }
            // Alineación
            if (col.alignment.value === 1) th.style.textAlign = 'center';
            else if (col.alignment.value === 2) th.style.textAlign = 'right';
            else th.style.textAlign = 'left';

            tr.appendChild(th);
        }
        columns.delete(); // Importante liberar vector de Embind
        this.thead.appendChild(tr);
    }

    renderRows() {
        this.tbody.innerHTML = '';
        const count = this.vm.getRowCount();
        console.log('renderRows called, row count:', count);

        for (let i = 0; i < count; i++) {
            const rowData = this.vm.getRow(i); // DcGridRow
            const tr = document.createElement('tr');

            if (rowData.isGhost) {
                tr.classList.add('dc-row-ghost');
            }

            // Render Celdas
            const cells = rowData.cells; // Vector<string>
            const cellCount = cells.size();

            for (let c = 0; c < cellCount; c++) {
                const td = document.createElement('td');
                td.innerText = cells.get(c);
                tr.appendChild(td);
            }
            cells.delete();

            // Eventos de Fila
            tr.addEventListener('click', () => {
                console.log('Row clicked:', i, 'InputLocked:', this.vm.isInputLocked());
                if (!this.vm.isInputLocked()) {
                    this.vm.selectRow(i);
                }
            });

            tr.addEventListener('dblclick', () => {
                console.log('Row double-clicked:', i);
                if (!this.vm.isInputLocked()) {
                    this.vm.activateRow();
                }
            });

            this.tbody.appendChild(tr);
            console.log('Row', i, 'added to tbody');
        }
    }

    updateSelectionVisuals(selectedIndex) {
        console.log('updateSelectionVisuals called, selectedIndex:', selectedIndex);
        const rows = this.tbody.querySelectorAll('tr');
        console.log('Found rows:', rows.length);
        rows.forEach((row, index) => {
            if (index === selectedIndex) {
                console.log('Adding selected class to row', index);
                row.classList.add('selected');
                row.scrollIntoView({ block: 'nearest' });
            } else {
                row.classList.remove('selected');
            }
        });
    }

    handleKeyDown(e) {
        if (this.vm.isInputLocked()) return;

        if (e.key === 'ArrowDown') {
            e.preventDefault();
            this.vm.moveSelection(1);
        } else if (e.key === 'ArrowUp') {
            e.preventDefault();
            this.vm.moveSelection(-1);
        } else if (e.key === 'Enter') {
            e.preventDefault();
            this.vm.activateRow();
        }
    }
}