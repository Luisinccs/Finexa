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
void Moneda::setSiglas(const std::string &newSiglas) { siglas = newSiglas; }

std::string Moneda::getNombre() const { return nombre; }
void Moneda::setNombre(const std::string &newNombre) { nombre = newNombre; }

std::string Moneda::getSimbolo() const { return simbolo; }
void Moneda::setSimbolo(const std::string &newSimbolo) { simbolo = newSimbolo; }

bool Moneda::isDeletable() const { return _isDeletable; }
void Moneda::setIsDeletable(bool deletable) { _isDeletable = deletable; }

} // namespace Finexa
