/**
 * @file Operacion.h
 * @author Finexa Team
 * @date 2026-01-31
 * @brief Entidad que representa un movimiento o registro financiero.
 */

#ifndef FINEXA_OPERACION_H
#define FINEXA_OPERACION_H

#include "Moneda.h"
#include <memory>
#include <string>

namespace Finexa {
/**
 * @class Operacion
 * @brief Modela una transaccion con su monto original y su equivalente en la
 * moneda base del sistema (XDS).
 */
class Operacion {
private:
  std::string uuid;
  std::string concepto;
  double montoOriginal;
  std::shared_ptr<Moneda> monedaOriginal;
  double montoXds;

public:
  Operacion(std::string concepto, double monto, std::shared_ptr<Moneda> moneda,
            double xds = 0.0);
  Operacion(std::string uuid, std::string concepto, double monto,
            std::shared_ptr<Moneda> moneda, double xds = 0.0);

  std::string getUuid() const;
  void setUuid(const std::string &newUuid);
  std::string getConcepto() const;
  double getMontoOriginal() const;
  std::shared_ptr<Moneda> getMonedaOriginal() const;
  double getMontoXds() const;

  void setConcepto(const std::string &nuevoConcepto);
  void setMontoOriginal(double nuevoMonto);
  void setMonedaOriginal(std::shared_ptr<Moneda> nuevaMoneda);
  void setMontoXds(double nuevoXds);
};
} // namespace Finexa

#endif
