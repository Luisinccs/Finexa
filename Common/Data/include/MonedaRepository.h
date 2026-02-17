/**
 * @file MonedaRepository.h
 * @date 2026-02-16
 * @brief Repositorio para persistir entidades Moneda en SQLite.
 */

#ifndef FINEXA_MONEDA_REPOSITORY_H
#define FINEXA_MONEDA_REPOSITORY_H

#include "../../Core/include/Moneda.h"
#include <memory>
#include <string>
#include <vector>

namespace Finexa::Data {

class MonedaRepository {
public:
  static std::vector<std::shared_ptr<Moneda>> getAll();
  static void save(std::shared_ptr<Moneda> moneda);
  static void remove(const std::string &uuid);
};

} // namespace Finexa::Data

#endif
