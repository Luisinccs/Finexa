#include "../include/TasaDeCambio.h"

namespace Finexa {

TasaDeCambio::TasaDeCambio(std::shared_ptr<Moneda> base,
                           std::shared_ptr<Moneda> destino, double valor)
    : monedaBase(base), monedaDestino(destino), valor(valor) {}

TasaDeCambio::TasaDeCambio(std::string uuid, std::shared_ptr<Moneda> base,
                           std::shared_ptr<Moneda> destino, double valor)
    : uuid(uuid), monedaBase(base), monedaDestino(destino), valor(valor) {}

std::string TasaDeCambio::getUuid() const { return uuid; }

void TasaDeCambio::setUuid(const std::string &newUuid) { uuid = newUuid; }

std::shared_ptr<Moneda> TasaDeCambio::getBase() const { return monedaBase; }

std::shared_ptr<Moneda> TasaDeCambio::getDestino() const {
  return monedaDestino;
}

double TasaDeCambio::getValor() const { return valor; }

void TasaDeCambio::setValor(double nuevoValor) { valor = nuevoValor; }

std::string TasaDeCambio::getParidad() const {
  return monedaBase->getSiglas() + "/" + monedaDestino->getSiglas();
}

} // namespace Finexa
