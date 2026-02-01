/**
 * @file TasaDeCambio.h
 * @author Finexa Team
 * @date 2026-01-31
 * @brief Entidad que representa la tasa de cambio entre dos monedas.
 */

#ifndef FINEXA_TASA_DE_CAMBIO_H
#define FINEXA_TASA_DE_CAMBIO_H

#include "Moneda.h"
#include <memory>
#include <string>

namespace Finexa {
    /**
     * @class TasaDeCambio
     * @brief Representa la paridad cambiaria entre una moneda base y una moneda de cotizacion.
     */
    class TasaDeCambio {
    private:
        std::shared_ptr<Moneda> monedaBase;
        std::shared_ptr<Moneda> monedaDestino;
        double valor;

    public:
        TasaDeCambio(std::shared_ptr<Moneda> base, std::shared_ptr<Moneda> destino, double valor);

        std::shared_ptr<Moneda> getBase() const;
        std::shared_ptr<Moneda> getDestino() const;
        double getValor() const;
        
        void setValor(double nuevoValor);
        std::string getParidad() const;
    };
}

#endif
