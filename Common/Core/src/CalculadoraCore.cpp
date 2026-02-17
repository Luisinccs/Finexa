#include "../include/CalculadoraCore.h"
#include "../../Data/include/MonedaRepository.h"
#include "../../Data/include/OperacionRepository.h"
#include "../../Data/include/TasaRepository.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace Finexa {

std::shared_ptr<Moneda>
CalculadoraCore::buscarMoneda(const std::string &siglas) {
  auto it = std::find_if(monedas.begin(), monedas.end(), [&](const auto &m) {
    return m->getSiglas() == siglas;
  });
  return (it != monedas.end()) ? *it : nullptr;
}

void CalculadoraCore::registrarMoneda(std::shared_ptr<Moneda> moneda) {
  if (!buscarMoneda(moneda->getSiglas())) {
    monedas.push_back(moneda);
  }
}

std::shared_ptr<TasaDeCambio>
CalculadoraCore::buscarTasa(const std::string &base,
                            const std::string &destino) {
  auto it = std::find_if(tasas.begin(), tasas.end(), [&](const auto &t) {
    return t->getBase()->getSiglas() == base &&
           t->getDestino()->getSiglas() == destino;
  });
  return (it != tasas.end()) ? *it : nullptr;
}

void CalculadoraCore::establecerTasa(const std::string &siglasBase,
                                     const std::string &siglasDestino,
                                     double valor) {
  auto mBase = buscarMoneda(siglasBase);
  auto mDestino = buscarMoneda(siglasDestino);

  if (!mBase || !mDestino) {
    throw std::runtime_error("Una o ambas monedas no estan registradas.");
  }

  auto tasaExistente = buscarTasa(siglasBase, siglasDestino);
  if (tasaExistente) {
    tasaExistente->setValor(valor);
  } else {
    tasas.push_back(std::make_shared<TasaDeCambio>(mBase, mDestino, valor));
  }
}

double CalculadoraCore::calcularValorImplicito(const std::string &base,
                                               const std::string &destino) {
  if (base == destino)
    return 1.0;

  // Triangulacion: (VES/Destino) / (VES/Base)
  double tasaVesBase = 0.0;
  double tasaVesDestino = 0.0;

  if (base == PIVOTE)
    tasaVesBase = 1.0;
  else {
    auto t = buscarTasa(PIVOTE, base);
    if (t)
      tasaVesBase = t->getValor();
  }

  if (destino == PIVOTE)
    tasaVesDestino = 1.0;
  else {
    auto t = buscarTasa(PIVOTE, destino);
    if (t)
      tasaVesDestino = t->getValor();
  }

  if (tasaVesBase == 0.0)
    return 0.0;
  return tasaVesDestino / tasaVesBase;
}

double CalculadoraCore::convertirAXds(double monto,
                                      const std::string &siglasOrigen) {
  if (siglasOrigen == OBJETIVO)
    return monto;

  double tasaVesOrigen = 1.0;
  if (siglasOrigen != PIVOTE) {
    auto t = buscarTasa(PIVOTE, siglasOrigen);
    if (!t)
      return 0.0;
    tasaVesOrigen = t->getValor();
  }

  auto tasaVesXds = buscarTasa(PIVOTE, OBJETIVO);
  if (!tasaVesXds || tasaVesXds->getValor() == 0.0)
    return 0.0;

  double montoVes = monto * tasaVesOrigen;
  return montoVes / tasaVesXds->getValor();
}

bool CalculadoraCore::tieneAmbiguedad(const std::string &base,
                                      const std::string &destino,
                                      double valorManual) {
  double valorCalculado = calcularValorImplicito(base, destino);
  if (valorCalculado == 0.0)
    return false;

  return std::abs(valorManual - valorCalculado) > 0.001;
}

std::string CalculadoraCore::getSiglasPivote() const { return PIVOTE; }
std::string CalculadoraCore::getSiglasObjetivo() const { return OBJETIVO; }
const std::vector<std::shared_ptr<Moneda>> &
CalculadoraCore::getMonedas() const {
  return monedas;
}
const std::vector<std::shared_ptr<TasaDeCambio>> &
CalculadoraCore::getTasas() const {
  return tasas;
}

// =========================================================
// Persistencia
// =========================================================

void CalculadoraCore::cargarDesdeBD() {
  monedas = Data::MonedaRepository::getAll();
  tasas = Data::TasaRepository::getAll(monedas);
}

void CalculadoraCore::guardarMoneda(std::shared_ptr<Moneda> moneda) {
  Data::MonedaRepository::save(moneda);
  if (!buscarMoneda(moneda->getSiglas())) {
    monedas.push_back(moneda);
  }
}

void CalculadoraCore::guardarTasa(std::shared_ptr<TasaDeCambio> tasa) {
  Data::TasaRepository::save(tasa);
  auto existente =
      buscarTasa(tasa->getBase()->getSiglas(), tasa->getDestino()->getSiglas());
  if (!existente) {
    tasas.push_back(tasa);
  }
}

void CalculadoraCore::eliminarMoneda(const std::string &uuid) {
  Data::MonedaRepository::remove(uuid);
  monedas.erase(
      std::remove_if(monedas.begin(), monedas.end(),
                     [&](const auto &m) { return m->getUuid() == uuid; }),
      monedas.end());
}

void CalculadoraCore::eliminarTasa(const std::string &uuid) {
  Data::TasaRepository::remove(uuid);
  tasas.erase(
      std::remove_if(tasas.begin(), tasas.end(),
                     [&](const auto &t) { return t->getUuid() == uuid; }),
      tasas.end());
}

} // namespace Finexa
