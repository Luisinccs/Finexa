#include "../include/TasaDeCambio.h"

namespace Finexa {

    TasaDeCambio::TasaDeCambio(std::shared_ptr<Moneda> base, std::shared_ptr<Moneda> destino, double valor)
        : monedaBase(base), monedaDestino(destino), valor(valor) {}

    std::shared_ptr<Moneda> TasaDeCambio::getBase() const {
        return monedaBase;
    }

    std::shared_ptr<Moneda> TasaDeCambio::getDestino() const {
        return monedaDestino;
    }

    double TasaDeCambio::getValor() const {
        return valor;
    }

    void TasaDeCambio::setValor(double nuevoValor) {
        valor = nuevoValor;
    }

    std::string TasaDeCambio::getParidad() const {
        return monedaBase->getSiglas() + "/" + monedaDestino->getSiglas();
    }

}
