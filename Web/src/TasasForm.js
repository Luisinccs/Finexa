
import { DcGrid, DcNumberTextField, DcComboBox, DcButton } from './dc-views.js';

export class TasasForm extends HTMLElement {
    constructor() {
        super();
        this.vm = null;
        this.bindings = {};
    }

    async initialize(commonModule, parentVm) {
        if (parentVm && parentVm.getTasasVm) {
            this.vm = parentVm.getTasasVm();
        } else {
            this.vm = commonModule.TasasViewModel.create();
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
                    grid-template-columns: 1fr 1fr 1fr auto;
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
                        <label>Moneda Base</label>
                        <div id="base-container"></div>
                    </div>
                    <div class="field-group">
                        <label>Moneda Destino</label>
                        <div id="destino-container"></div>
                    </div>
                    <div class="field-group">
                        <label>Tasa / Valor</label>
                        <div id="valor-container"></div>
                    </div>
                    <div id="btn-container"></div>
                </div>
                <div class="grid-panel" id="grid-container"></div>
            </div>
        `;
    }

    bindComponents() {
        // Grid
        const gridVm = this.vm.gridTasas();
        this.bindings.grid = new DcGrid(gridVm);
        this.querySelector('#grid-container').appendChild(this.bindings.grid.element);

        // Selector Base (ComboBox)
        const baseVm = this.vm.selectorBase();
        this.bindings.base = new DcComboBox(baseVm);
        this.querySelector('#base-container').appendChild(this.bindings.base.element);

        // Selector Destino (ComboBox)
        const destinoVm = this.vm.selectorDestino();
        this.bindings.destino = new DcComboBox(destinoVm);
        this.querySelector('#destino-container').appendChild(this.bindings.destino.element);

        // Valor (NumberField)
        const valorVm = this.vm.inputValor();
        this.bindings.valor = new DcNumberTextField(valorVm);
        this.querySelector('#valor-container').appendChild(this.bindings.valor.element);

        // Botón Guardar
        const btnVm = this.vm.cmdGuardarTasa();
        this.bindings.btn = new DcButton(btnVm);
        this.querySelector('#btn-container').appendChild(this.bindings.btn.element);
    }

    disconnectedCallback() {
        Object.values(this.bindings).forEach(b => {
            if (b && b.dispose) b.dispose();
        });
    }
}

customElements.define('tasas-form', TasasForm);
