#include "../include/ConfigRepository.h"
#include "DcDataService.hpp"

namespace Finexa::Data {

std::string ConfigRepository::get(const std::string &key,
                                  const std::string &defaultValue) {
  auto db = DualComponents::Data::DcDataService::get();
  if (!db)
    return defaultValue;

  DualComponents::Data::DcParams params;
  params[":key"] = key;

  auto rows =
      db->query("SELECT value FROM configuracion WHERE key = :key", params);
  if (!rows.empty()) {
    return std::get<std::string>(rows[0].at("value"));
  }
  return defaultValue;
}

void ConfigRepository::set(const std::string &key, const std::string &value) {
  auto db = DualComponents::Data::DcDataService::get();
  if (!db)
    return;

  DualComponents::Data::DcParams params;
  params[":key"] = key;
  params[":value"] = value;

  db->execute(
      "INSERT OR REPLACE INTO configuracion (key, value) VALUES (:key, :value)",
      params);
}

} // namespace Finexa::Data
