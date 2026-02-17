#include "../include/Moneda.h"

namespace Finexa {

Moneda::Moneda(int id, std::string siglas, std::string nombre,
               std::string simbolo)
    : id(id), siglas(siglas), nombre(nombre), simbolo(simbolo) {}

Moneda::Moneda(std::string uuid, int id, std::string siglas, std::string nombre,
               std::string simbolo)
    : uuid(uuid), id(id), siglas(siglas), nombre(nombre), simbolo(simbolo) {}

int Moneda::getId() const { return id; }

std::string Moneda::getUuid() const { return uuid; }

void Moneda::setUuid(const std::string &newUuid) { uuid = newUuid; }

std::string Moneda::getSiglas() const { return siglas; }

std::string Moneda::getNombre() const { return nombre; }

std::string Moneda::getSimbolo() const { return simbolo; }

} // namespace Finexa
