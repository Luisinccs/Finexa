#include "../include/MonedaRepository.h"
#include "DcDataService.hpp"
#include "UuidGenerator.hpp"
#include <chrono>

namespace Finexa::Data {

static int64_t now() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

std::vector<std::shared_ptr<Moneda>> MonedaRepository::getAll() {
  std::vector<std::shared_ptr<Moneda>> result;
  auto db = DualComponents::Data::DcDataService::get();
  if (!db)
    return result;

  auto rows = db->query("SELECT id, siglas, nombre, simbolo FROM monedas WHERE "
                        "is_deleted = 0 ORDER BY siglas");

  int localId = 1;
  for (const auto &row : rows) {
    std::string uuid = std::get<std::string>(row.at("id"));
    std::string siglas = std::get<std::string>(row.at("siglas"));
    std::string nombre = std::get<std::string>(row.at("nombre"));
    std::string simbolo = std::get<std::string>(row.at("simbolo"));

    result.push_back(
        std::make_shared<Moneda>(uuid, localId++, siglas, nombre, simbolo));
  }

  return result;
}

void MonedaRepository::save(std::shared_ptr<Moneda> moneda) {
  auto db = DualComponents::Data::DcDataService::get();
  if (!db)
    return;

  std::string uuid = moneda->getUuid();
  int64_t ts = now();

  if (uuid.empty()) {
    // INSERT — nuevo registro
    uuid = DualComponents::Helpers::UuidGenerator::v7();
    moneda->setUuid(uuid);

    DualComponents::Data::DcParams params;
    params[":id"] = uuid;
    params[":siglas"] = moneda->getSiglas();
    params[":nombre"] = moneda->getNombre();
    params[":simbolo"] = moneda->getSimbolo();
    params[":created_at"] = ts;
    params[":updated_at"] = ts;

    db->execute(
        "INSERT INTO monedas (id, siglas, nombre, simbolo, created_at, "
        "updated_at) "
        "VALUES (:id, :siglas, :nombre, :simbolo, :created_at, :updated_at)",
        params);
  } else {
    // UPDATE — registro existente
    DualComponents::Data::DcParams params;
    params[":id"] = uuid;
    params[":siglas"] = moneda->getSiglas();
    params[":nombre"] = moneda->getNombre();
    params[":simbolo"] = moneda->getSimbolo();
    params[":updated_at"] = ts;

    db->execute("UPDATE monedas SET siglas = :siglas, nombre = :nombre, "
                "simbolo = :simbolo, "
                "updated_at = :updated_at WHERE id = :id",
                params);
  }
}

void MonedaRepository::remove(const std::string &uuid) {
  auto db = DualComponents::Data::DcDataService::get();
  if (!db)
    return;

  DualComponents::Data::DcParams params;
  params[":id"] = uuid;
  params[":updated_at"] = now();

  db->execute("UPDATE monedas SET is_deleted = 1, updated_at = :updated_at "
              "WHERE id = :id",
              params);
}

} // namespace Finexa::Data
