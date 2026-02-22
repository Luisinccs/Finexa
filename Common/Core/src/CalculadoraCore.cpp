#include "../include/CalculadoraCore.h"
#include "../../Data/include/MonedaRepository.h"
#include "../../Data/include/OperacionRepository.h"
#include "../../Data/include/TasaRepository.h"
// Relative path to DualComponents from Common/Core/src
// #include "../../../../DualComponents/DcDataAccess/Core/DcDataService.hpp"
#include "DcDataService.hpp"
#include <algorithm>
#include <cmath>
#include <queue>
#include <stdexcept>
#include <unordered_map>

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
    guardarTasa(tasaExistente); // Integrar persistencia
  } else {
    auto nueva = std::make_shared<TasaDeCambio>(mBase, mDestino, valor);
    // guardarTasa ya lo agrega a la lista si no existe
    guardarTasa(nueva);
  }
}

std::shared_ptr<Moneda>
CalculadoraCore::buscarMonedaPorUuid(const std::string &uuid) {
  auto it = std::find_if(monedas.begin(), monedas.end(),
                         [&](const auto &m) { return m->getUuid() == uuid; });
  return (it != monedas.end()) ? *it : nullptr;
}

void CalculadoraCore::establecerTasaPorUuid(const std::string &uuidBase,
                                            const std::string &uuidDestino,
                                            double valor) {
  auto mBase = buscarMonedaPorUuid(uuidBase);
  auto mDestino = buscarMonedaPorUuid(uuidDestino);

  if (!mBase || !mDestino) {
    throw std::runtime_error("Una o ambas monedas no estan registradas.");
  }

  // Buscar tasa existente por siglas (unico identificador logico por ahora)
  auto tasaExistente = buscarTasa(mBase->getSiglas(), mDestino->getSiglas());

  if (tasaExistente) {
    tasaExistente->setValor(valor);
    guardarTasa(tasaExistente);
  } else {
    auto nueva = std::make_shared<TasaDeCambio>(mBase, mDestino, valor);
    guardarTasa(nueva);
  }
}

double CalculadoraCore::calcularValorImplicito(const std::string &base,
                                               const std::string &destino) {
  if (base == destino)
    return 1.0;

  // BFS to find a conversion path from base to destino
  std::unordered_map<std::string, double> visited;
  std::queue<std::string> q;

  visited[base] = 1.0;
  q.push(base);

  while (!q.empty()) {
    std::string current = q.front();
    q.pop();

    if (current == destino) {
      return visited[current];
    }

    // Try all possible direct rates from 'current' to any other node
    for (const auto &t : tasas) {
      std::string tBase = t->getBase()->getSiglas();
      std::string tDestino = t->getDestino()->getSiglas();
      double valorTasa = t->getValor();

      if (tBase == current) {
        // Para ir de Base (ej. VES) a Destino (ej. USD):
        // Como 1 Destino = valorTasa * Base, entonces 1 Base = (1 / valorTasa) Destino
        if (visited.find(tDestino) == visited.end() && valorTasa > 0) {
          visited[tDestino] = visited[current] * (1.0 / valorTasa);
          q.push(tDestino);
        }
      } else if (tDestino == current) {
        // Para ir de Destino (ej. USD) a Base (ej. VES):
        // Como 1 Destino = valorTasa * Base, multiplicamos por la tasa.
        if (visited.find(tBase) == visited.end()) {
          visited[tBase] = visited[current] * valorTasa;
          q.push(tBase);
        }
      }
    }
  }

  return 0.0; // Path not found
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
const std::vector<std::shared_ptr<Operacion>> &
CalculadoraCore::getOperaciones() const {
  return operaciones;
}

// =========================================================
// Persistencia
// =========================================================

void CalculadoraCore::cargarDesdeBD() {
  monedas = Data::MonedaRepository::getAll();
  tasas = Data::TasaRepository::getAll(monedas);
  operaciones = Data::OperacionRepository::getAll(monedas);
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

void CalculadoraCore::guardarOperacion(std::shared_ptr<Operacion> operacion) {
  Data::OperacionRepository::save(operacion);
  operaciones.erase(std::remove_if(operaciones.begin(), operaciones.end(),
                                   [&](const auto &o) {
                                     return o->getUuid() ==
                                            operacion->getUuid();
                                   }),
                    operaciones.end());
  operaciones.push_back(operacion);
}

void CalculadoraCore::eliminarOperacion(const std::string &uuid) {
  Data::OperacionRepository::remove(uuid);
  operaciones.erase(
      std::remove_if(operaciones.begin(), operaciones.end(),
                     [&](const auto &o) { return o->getUuid() == uuid; }),
      operaciones.end());
}

// =========================================================
// Herramientas de Debug (Modo Dios)
// =========================================================

void CalculadoraCore::limpiarBaseDeDatos() {
  auto db = DualComponents::Data::DcDataService::get();
  if (db) {
    // Borrado duro para resetear todo
    db->execute("DELETE FROM tasas");
    db->execute("DELETE FROM operaciones");
    db->execute("DELETE FROM monedas"); // Orden por FK constraints
  }

  // Limpiar memoria
  monedas.clear();
  tasas.clear();
  operaciones.clear();
}

void CalculadoraCore::cargarDatosMock() {
  // 1. Limpiar primero para evitar duplicados en este modo de prueba
  limpiarBaseDeDatos();

  // 2. Crear Monedas Mock
  auto usd = std::make_shared<Moneda>(1, "USD", "Dolar Americano", "$");
  auto ves = std::make_shared<Moneda>(2, "VES", "Bolivar", "Bs");
  auto eur = std::make_shared<Moneda>(3, "EUR", "Euro", "€");
  auto bcv = std::make_shared<Moneda>(4, "REF", "Dolar BCV", "$B");

  guardarMoneda(usd);
  guardarMoneda(ves);
  guardarMoneda(eur);
  guardarMoneda(bcv);

  // 3. Crear Tasas Mock
  // USD -> VES (50)
  auto t1 = std::make_shared<TasaDeCambio>(ves, usd, 520.0);
  guardarTasa(t1);

  // EUR -> USD (1.10)
  auto t2 = std::make_shared<TasaDeCambio>(eur, usd, 1.10);
  guardarTasa(t2);

  // EUR -> VES (55)
  auto t3 = std::make_shared<TasaDeCambio>(ves, bcv, 380.0);
  guardarTasa(t3);
}

} // namespace Finexa
