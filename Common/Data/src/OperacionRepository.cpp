#include "../include/OperacionRepository.h"
#include "DcDataService.hpp"
#include "UuidGenerator.hpp"
#include <algorithm>
#include <chrono>

namespace Finexa::Data {

static int64_t opNow() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

static std::shared_ptr<Moneda>
findMonedaByUuidOp(const std::vector<std::shared_ptr<Moneda>> &monedas,
                   const std::string &uuid) {
  auto it = std::find_if(monedas.begin(), monedas.end(),
                         [&](const auto &m) { return m->getUuid() == uuid; });
  return (it != monedas.end()) ? *it : nullptr;
}

std::vector<std::shared_ptr<Operacion>> OperacionRepository::getAll(
    const std::vector<std::shared_ptr<Moneda>> &monedas) {

  std::vector<std::shared_ptr<Operacion>> result;
  auto db = DualComponents::Data::DcDataService::get();
  if (!db)
    return result;

  auto rows =
      db->query("SELECT id, concepto, monto_original, moneda_id, monto_xds "
                "FROM operaciones WHERE is_deleted = 0 ORDER BY created_at");

  for (const auto &row : rows) {
    std::string uuid = std::get<std::string>(row.at("id"));
    std::string concepto = std::get<std::string>(row.at("concepto"));
    double montoOrig = std::get<double>(row.at("monto_original"));
    std::string monedaId = std::get<std::string>(row.at("moneda_id"));
    double montoXds = std::get<double>(row.at("monto_xds"));

    auto moneda = findMonedaByUuidOp(monedas, monedaId);
    if (moneda) {
      result.push_back(std::make_shared<Operacion>(uuid, concepto, montoOrig,
                                                   moneda, montoXds));
    }
  }

  return result;
}

void OperacionRepository::save(std::shared_ptr<Operacion> operacion) {
  auto db = DualComponents::Data::DcDataService::get();
  if (!db)
    return;

  std::string uuid = operacion->getUuid();
  int64_t ts = opNow();

  if (uuid.empty()) {
    uuid = DualComponents::Helpers::UuidGenerator::v7();
    operacion->setUuid(uuid);

    DualComponents::Data::DcParams params;
    params[":id"] = uuid;
    params[":concepto"] = operacion->getConcepto();
    params[":monto_original"] = operacion->getMontoOriginal();
    params[":moneda_id"] = operacion->getMonedaOriginal()->getUuid();
    params[":monto_xds"] = operacion->getMontoXds();
    params[":created_at"] = ts;
    params[":updated_at"] = ts;

    db->execute("INSERT INTO operaciones (id, concepto, monto_original, "
                "moneda_id, monto_xds, created_at, updated_at) "
                "VALUES (:id, :concepto, :monto_original, :moneda_id, "
                ":monto_xds, :created_at, :updated_at)",
                params);
  } else {
    DualComponents::Data::DcParams params;
    params[":id"] = uuid;
    params[":concepto"] = operacion->getConcepto();
    params[":monto_original"] = operacion->getMontoOriginal();
    params[":moneda_id"] = operacion->getMonedaOriginal()->getUuid();
    params[":monto_xds"] = operacion->getMontoXds();
    params[":updated_at"] = ts;

    db->execute("UPDATE operaciones SET concepto = :concepto, monto_original = "
                ":monto_original, "
                "moneda_id = :moneda_id, monto_xds = :monto_xds, updated_at = "
                ":updated_at "
                "WHERE id = :id",
                params);
  }
}

void OperacionRepository::remove(const std::string &uuid) {
  auto db = DualComponents::Data::DcDataService::get();
  if (!db)
    return;

  DualComponents::Data::DcParams params;
  params[":id"] = uuid;
  params[":updated_at"] = opNow();

  db->execute("UPDATE operaciones SET is_deleted = 1, updated_at = :updated_at "
              "WHERE id = :id",
              params);
}

} // namespace Finexa::Data
