/**
 * @file CalculadoraCore.h
 * @author Finexa Team
 * @date 2026-01-31
 * @brief Clase principal que gestiona la logica de conversion y triangulacion.
 */

#ifndef FINEXA_CALCULADORA_CORE_H
#define FINEXA_CALCULADORA_CORE_H

#include "Moneda.h"
#include "TasaDeCambio.h"
#include <vector>
#include <memory>
#include <string>

namespace Finexa {
    /**
     * @class CalculadoraCore
     * @brief Gestiona el inventario de monedas, tasas y realiza calculos de triangulacion.
     */
    class CalculadoraCore {
    private:
        std::vector<std::shared_ptr<Moneda>> monedas;
        std::vector<std::shared_ptr<TasaDeCambio>> tasas;
        
        const std::string PIVOTE = "VES";
        const std::string OBJETIVO = "XDS";

    public:
        CalculadoraCore() = default;

        std::shared_ptr<Moneda> buscarMoneda(const std::string& siglas);
        void registrarMoneda(std::shared_ptr<Moneda> moneda);

        std::shared_ptr<TasaDeCambio> buscarTasa(const std::string& base, const std::string& destino);
        void establecerTasa(const std::string& siglasBase, const std::string& siglasDestino, double valor);

        double calcularValorImplicito(const std::string& base, const std::string& destino);
        double convertirAXds(double monto, const std::string& siglasOrigen);
        bool tieneAmbiguedad(const std::string& base, const std::string& destino, double valorManual);

        std::string getSiglasPivote() const;
        std::string getSiglasObjetivo() const;
        const std::vector<std::shared_ptr<Moneda>>& getMonedas() const;
        const std::vector<std::shared_ptr<TasaDeCambio>>& getTasas() const;
    };
}

#endif
