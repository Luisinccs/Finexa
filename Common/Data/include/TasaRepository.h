/**
 * @file TasaRepository.h
 * @date 2026-02-16
 * @brief Repositorio para persistir entidades TasaDeCambio en SQLite.
 */

#ifndef FINEXA_TASA_REPOSITORY_H
#define FINEXA_TASA_REPOSITORY_H

#include "../../Core/include/Moneda.h"
#include "../../Core/include/TasaDeCambio.h"
#include <memory>
#include <string>
#include <vector>

namespace Finexa::Data {

class TasaRepository {
public:
  static std::vector<std::shared_ptr<TasaDeCambio>>
  getAll(const std::vector<std::shared_ptr<Moneda>> &monedas);
  static void save(std::shared_ptr<TasaDeCambio> tasa);
  static void remove(const std::string &uuid);
};

} // namespace Finexa::Data

#endif
