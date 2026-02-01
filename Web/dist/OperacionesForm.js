
import { DcGrid, DcNumberTextField, DcInput, DcComboBox, DcButton } from './dc-views.js';

export class OperacionesForm extends HTMLElement {
    constructor() {
        super();
        this.vm = null;
        this.bindings = {};
    }

    async initialize(commonModule, parentVm) {
        // En lugar de crear el VM, lo obtenemos del MainViewModel si se proporciona,
        // o lo creamos si no. Pero segun la arquitectura, el Main lo tiene.
        if (parentVm && parentVm.getOperacionesVm) {
            this.vm = parentVm.getOperacionesVm();
        } else {
            this.vm = commonModule.OperacionesViewModel.create();
        }

        this.render();
        this.bindComponents();

        if (this.vm.inicializar) this.vm.inicializar();
    }

    render() {
        this.innerHTML = `
            <style>
                .form-container {
                    display: flex;
                    flex-direction: column;
                    gap: 1rem;
                    height: 100%;
                }
                .editor-panel {
                    display: grid;
                    grid-template-columns: 2fr 1fr 1fr auto;
                    gap: 1rem;
                    align-items: end;
                    background: #252525;
                    padding: 1rem;
                    border-radius: 4px;
                    border: 1px solid #444;
                }
                .grid-panel {
                    flex: 1;
                    min-height: 0;
                    border: 1px solid #444;
                    border-radius: 4px;
                    overflow: hidden;
                }
                .field-group {
                    display: flex;
                    flex-direction: column;
                    gap: 0.3rem;
                }
                label {
                    font-size: 0.8rem;
                    color: #aaa;
                }
            </style>
            <div class="form-container">
                <div class="editor-panel">
                    <div class="field-group">
                        <label>Concepto</label>
                        <div id="concepto-container"></div>
                    </div>
                    <div class="field-group">
                        <label>Monto</label>
                        <div id="monto-container"></div>
                    </div>
                    <div class="field-group">
                        <label>Moneda</label>
                        <div id="moneda-container"></div>
                    </div>
                    <div id="btn-container"></div>
                </div>
                <div class="grid-panel" id="grid-container"></div>
            </div>
        `;
    }

    bindComponents() {
        // Grid
        const gridVm = this.vm.gridOperaciones();
        this.bindings.grid = new DcGrid(gridVm);
        this.querySelector('#grid-container').appendChild(this.bindings.grid.element);

        // Concepto (Input)
        const conceptoVm = this.vm.inputConcepto();
        this.bindings.concepto = new DcInput(conceptoVm);
        this.querySelector('#concepto-container').appendChild(this.bindings.concepto.element);

        // Monto (NumberField)
        const montoVm = this.vm.inputMonto();
        this.bindings.monto = new DcNumberTextField(montoVm);
        this.querySelector('#monto-container').appendChild(this.bindings.monto.element);

        // Moneda (ComboBox)
        const monedaVm = this.vm.selectorMoneda();
        this.bindings.moneda = new DcComboBox(monedaVm);
        this.querySelector('#moneda-container').appendChild(this.bindings.moneda.element);

        // Botón Agregar
        const btnVm = this.vm.cmdAgregar();
        this.bindings.btn = new DcButton(btnVm);
        this.querySelector('#btn-container').appendChild(this.bindings.btn.element);
    }

    disconnectedCallback() {
        Object.values(this.bindings).forEach(b => {
            if (b && b.dispose) b.dispose();
        });
    }
}

customElements.define('operaciones-form', OperacionesForm);
