#include "../include/TasaRepository.h"
#include "DcDataService.hpp"
#include "UuidGenerator.hpp"
#include <algorithm>
#include <chrono>

namespace Finexa::Data {

static int64_t tasaNow() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

// Helper: buscar moneda por uuid en la lista cargada
static std::shared_ptr<Moneda>
findMonedaByUuid(const std::vector<std::shared_ptr<Moneda>> &monedas,
                 const std::string &uuid) {
  auto it = std::find_if(monedas.begin(), monedas.end(),
                         [&](const auto &m) { return m->getUuid() == uuid; });
  return (it != monedas.end()) ? *it : nullptr;
}

std::vector<std::shared_ptr<TasaDeCambio>>
TasaRepository::getAll(const std::vector<std::shared_ptr<Moneda>> &monedas) {

  std::vector<std::shared_ptr<TasaDeCambio>> result;
  auto db = DualComponents::Data::DcDataService::get();
  if (!db)
    return result;

  auto rows = db->query("SELECT id, moneda_base_id, moneda_dest_id, valor "
                        "FROM tasas WHERE is_deleted = 0");

  for (const auto &row : rows) {
    std::string uuid = std::get<std::string>(row.at("id"));
    std::string baseId = std::get<std::string>(row.at("moneda_base_id"));
    std::string destId = std::get<std::string>(row.at("moneda_dest_id"));
    double valor = std::get<double>(row.at("valor"));

    auto mBase = findMonedaByUuid(monedas, baseId);
    auto mDest = findMonedaByUuid(monedas, destId);

    if (mBase && mDest) {
      result.push_back(
          std::make_shared<TasaDeCambio>(uuid, mBase, mDest, valor));
    }
  }

  return result;
}

void TasaRepository::save(std::shared_ptr<TasaDeCambio> tasa) {
  auto db = DualComponents::Data::DcDataService::get();
  if (!db)
    return;

  std::string uuid = tasa->getUuid();
  int64_t ts = tasaNow();

  if (uuid.empty()) {
    uuid = DualComponents::Helpers::UuidGenerator::v7();
    tasa->setUuid(uuid);

    DualComponents::Data::DcParams params;
    params[":id"] = uuid;
    params[":base_id"] = tasa->getBase()->getUuid();
    params[":dest_id"] = tasa->getDestino()->getUuid();
    params[":valor"] = tasa->getValor();
    params[":created_at"] = ts;
    params[":updated_at"] = ts;

    db->execute(
        "INSERT INTO tasas (id, moneda_base_id, moneda_dest_id, valor, "
        "created_at, updated_at) "
        "VALUES (:id, :base_id, :dest_id, :valor, :created_at, :updated_at)",
        params);
  } else {
    DualComponents::Data::DcParams params;
    params[":id"] = uuid;
    params[":valor"] = tasa->getValor();
    params[":updated_at"] = ts;

    db->execute("UPDATE tasas SET valor = :valor, updated_at = :updated_at "
                "WHERE id = :id",
                params);
  }
}

void TasaRepository::remove(const std::string &uuid) {
  auto db = DualComponents::Data::DcDataService::get();
  if (!db)
    return;

  DualComponents::Data::DcParams params;
  params[":id"] = uuid;
  params[":updated_at"] = tasaNow();

  db->execute("UPDATE tasas SET is_deleted = 1, updated_at = :updated_at WHERE "
              "id = :id",
              params);
}

} // namespace Finexa::Data
