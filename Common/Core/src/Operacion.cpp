#include "../include/Operacion.h"

namespace Finexa {

Operacion::Operacion(std::string concepto, double monto,
                     std::shared_ptr<Moneda> moneda, double xds)
    : concepto(concepto), montoOriginal(monto), monedaOriginal(moneda),
      montoXds(xds) {}

Operacion::Operacion(std::string uuid, std::string concepto, double monto,
                     std::shared_ptr<Moneda> moneda, double xds)
    : uuid(uuid), concepto(concepto), montoOriginal(monto),
      monedaOriginal(moneda), montoXds(xds) {}

std::string Operacion::getUuid() const { return uuid; }

void Operacion::setUuid(const std::string &newUuid) { uuid = newUuid; }

std::string Operacion::getConcepto() const { return concepto; }

double Operacion::getMontoOriginal() const { return montoOriginal; }

std::shared_ptr<Moneda> Operacion::getMonedaOriginal() const {
  return monedaOriginal;
}

double Operacion::getMontoXds() const { return montoXds; }

void Operacion::setConcepto(const std::string &nuevoConcepto) {
  concepto = nuevoConcepto;
}

void Operacion::setMontoOriginal(double nuevoMonto) {
  montoOriginal = nuevoMonto;
}

void Operacion::setMonedaOriginal(std::shared_ptr<Moneda> nuevaMoneda) {
  monedaOriginal = nuevaMoneda;
}

void Operacion::setMontoXds(double nuevoXds) { montoXds = nuevoXds; }

} // namespace Finexa
