/**
 * @file OperacionRepository.h
 * @date 2026-02-16
 * @brief Repositorio para persistir entidades Operacion en SQLite.
 */

#ifndef FINEXA_OPERACION_REPOSITORY_H
#define FINEXA_OPERACION_REPOSITORY_H

#include "../../Core/include/Moneda.h"
#include "../../Core/include/Operacion.h"
#include <memory>
#include <string>
#include <vector>

namespace Finexa::Data {

class OperacionRepository {
public:
  static std::vector<std::shared_ptr<Operacion>> getAll(const std::vector<std::shared_ptr<Moneda>> &monedas);
  static void save(std::shared_ptr<Operacion> operacion);
  static void remove(const std::string &uuid);
};

} // namespace Finexa::Data

#endif
