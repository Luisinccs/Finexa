import UIKit
import FinexaViewsIos

class MonedasControllerMockBinder: MonedasBinder {
    
    struct MonedaMock {
        var nombre: String
        var simbolo: String
        var siglas: String
    }
    
    private var monedas: [MonedaMock] = [
        MonedaMock(nombre: "Dólar Estadounidense", simbolo: "$", siglas: "USD"),
        MonedaMock(nombre: "Euro", simbolo: "€", siglas: "EUR"),
        MonedaMock(nombre: "Yen Japonés", simbolo: "¥", siglas: "JPY"),
        MonedaMock(nombre: "Libra Esterlina", simbolo: "£", siglas: "GBP")
    ]
    
    private var selectedIndex: Int?
    var onDataChanged: (() -> Void)?
    
    func numberOfRows() -> Int {
        return monedas.count
    }
    
    func cellData(for row: Int) -> (nombre: String, simbolo: String, siglas: String) {
        let m = monedas[row]
        return (m.nombre, m.simbolo, m.siglas)
    }
    
    func selectRow(at row: Int) {
        selectedIndex = row
    }
    
    func requestAdd() {
        selectedIndex = nil
    }
    
    func deleteRow(at row: Int) {
        monedas.remove(at: row)
        onDataChanged?()
    }
    
    func getEditorBinder() -> EditorMonedaBinder? {
        if let index = selectedIndex {
            return EditorMonedaMockBinder(moneda: monedas[index]) { [weak self] updated in
                self?.monedas[index] = updated
                self?.onDataChanged?()
            }
        } else {
            return EditorMonedaMockBinder(moneda: MonedaMock(nombre: "", simbolo: "", siglas: "")) { [weak self] newMoneda in
                self?.monedas.append(newMoneda)
                self?.onDataChanged?()
            }
        }
    }
}

class EditorMonedaMockBinder: EditorMonedaBinder {
    private var moneda: MonedasControllerMockBinder.MonedaMock
    private let onSave: (MonedasControllerMockBinder.MonedaMock) -> Void
    
    private weak var txtNombre: UITextField?
    private weak var txtSimbolo: UITextField?
    private weak var txtSiglas: UITextField?
    
    init(moneda: MonedasControllerMockBinder.MonedaMock, onSave: @escaping (MonedasControllerMockBinder.MonedaMock) -> Void) {
        self.moneda = moneda
        self.onSave = onSave
    }
    
    func bind(nombre: UITextField, simbolo: UITextField, siglas: UITextField) {
        self.txtNombre = nombre
        self.txtSimbolo = simbolo
        self.txtSiglas = siglas
        
        nombre.text = moneda.nombre
        simbolo.text = moneda.simbolo
        siglas.text = moneda.siglas
    }
    
    func accept() {
        let updated = MonedasControllerMockBinder.MonedaMock(
            nombre: txtNombre?.text ?? "",
            simbolo: txtSimbolo?.text ?? "",
            siglas: txtSiglas?.text ?? ""
        )
        onSave(updated)
    }
    
    func cancel() {
        // No-op for mock
    }
}
