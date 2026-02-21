#include "../include/DatabaseInitializer.h"
#include "DcDataService.hpp"
#include "SQLiteConnector.hpp"
#include <iostream>

namespace Finexa::Data {

void DatabaseInitializer::setup(const std::string &dbPath) {

  // 1. Crear conector SQLite
  auto connector =
      DualComponents::Data::SQLite::SQLiteConnector::create(dbPath);

  // 2. Registrar en el servicio global
  DualComponents::Data::DcDataService::initialize(connector);

  // 3. Crear tablas
  crearTablas();

  // [todo]: quitar al momento del deploy
  std::cout << "[Finexa] Base de datos inicializada en: " << dbPath
            << std::endl;
}

void DatabaseInitializer::crearTablas() {

  auto db = DualComponents::Data::DcDataService::get();

  if (!db)
    return;

  // Tabla: monedas
  db->execute("CREATE TABLE IF NOT EXISTS monedas ("
              "  id          TEXT PRIMARY KEY,"
              "  siglas      TEXT NOT NULL UNIQUE,"
              "  nombre      TEXT NOT NULL,"
              "  simbolo     TEXT NOT NULL,"
              "  created_at  INTEGER NOT NULL,"
              "  updated_at  INTEGER NOT NULL,"
              "  is_deleted  INTEGER NOT NULL DEFAULT 0,"
              "  sync_status INTEGER NOT NULL DEFAULT 1"
              ")");

  // Tabla: configuracion
  db->execute("CREATE TABLE IF NOT EXISTS configuracion ("
              "  key TEXT PRIMARY KEY,"
              "  value TEXT NOT NULL"
              ")");

  // Tabla: tasas
  db->execute("CREATE TABLE IF NOT EXISTS tasas ("
              "  id              TEXT PRIMARY KEY,"
              "  moneda_base_id  TEXT NOT NULL,"
              "  moneda_dest_id  TEXT NOT NULL,"
              "  valor           REAL NOT NULL,"
              "  created_at      INTEGER NOT NULL,"
              "  updated_at      INTEGER NOT NULL,"
              "  is_deleted      INTEGER NOT NULL DEFAULT 0,"
              "  sync_status     INTEGER NOT NULL DEFAULT 1,"
              "  UNIQUE(moneda_base_id, moneda_dest_id),"
              "  FOREIGN KEY (moneda_base_id) REFERENCES monedas(id),"
              "  FOREIGN KEY (moneda_dest_id) REFERENCES monedas(id)"
              ")");

  // Tabla: operaciones
  db->execute("CREATE TABLE IF NOT EXISTS operaciones ("
              "  id              TEXT PRIMARY KEY,"
              "  concepto        TEXT NOT NULL,"
              "  monto_original  REAL NOT NULL,"
              "  moneda_id       TEXT NOT NULL,"
              "  monto_xds       REAL NOT NULL DEFAULT 0.0,"
              "  created_at      INTEGER NOT NULL,"
              "  updated_at      INTEGER NOT NULL,"
              "  is_deleted      INTEGER NOT NULL DEFAULT 0,"
              "  sync_status     INTEGER NOT NULL DEFAULT 1,"
              "  FOREIGN KEY (moneda_id) REFERENCES monedas(id)"
              ")");
}

} // namespace Finexa::Data

// C-Bridge
void Finexa_DatabaseInitializer_setup(const char *dbPath) {
  if (dbPath) {
    Finexa::Data::DatabaseInitializer::setup(std::string(dbPath));
  }
}
