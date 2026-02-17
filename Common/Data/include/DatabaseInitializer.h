/**
 * @file DatabaseInitializer.h
 * @date 2026-02-16
 * @brief Responsable de inicializar la base de datos SQLite y crear las tablas.
 */

#ifndef FINEXA_DATABASE_INITIALIZER_H
#define FINEXA_DATABASE_INITIALIZER_H

#include <string>

namespace Finexa::Data {

class DatabaseInitializer {
public:
  /**
   * @brief Inicializa la base de datos: crea el conector SQLite,
   * lo registra en DcDataService y ejecuta los DDL de creación de tablas.
   * @param dbPath Ruta absoluta al archivo .db
   */
  static void setup(const std::string &dbPath);

private:
  static void crearTablas();
};

} // namespace Finexa::Data

// C-Bridge para Swift
#ifdef __cplusplus
extern "C" {
#endif

void Finexa_DatabaseInitializer_setup(const char *dbPath);

#ifdef __cplusplus
}
#endif

#endif
