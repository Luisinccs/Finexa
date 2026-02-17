/**
 * @file CalculadoraCore.h
 * @author Finexa Team
 * @date 2026-01-31
 * @brief Clase principal que gestiona la logica de conversion y triangulacion.
 */

#ifndef FINEXA_CALCULADORA_CORE_H
#define FINEXA_CALCULADORA_CORE_H

#include "Moneda.h"
#include "TasaDeCambio.h"
#include <memory>
#include <string>
#include <vector>

namespace Finexa {
/**
 * @class CalculadoraCore
 * @brief Gestiona el inventario de monedas, tasas y realiza calculos de
 * triangulacion.
 */
class CalculadoraCore {
private:
  std::vector<std::shared_ptr<Moneda>> monedas;
  std::vector<std::shared_ptr<TasaDeCambio>> tasas;

  const std::string PIVOTE = "VES";
  const std::string OBJETIVO = "XDS";

public:
  CalculadoraCore() = default;

  // --- Persistencia ---
  void cargarDesdeBD();
  void guardarMoneda(std::shared_ptr<Moneda> moneda);
  void guardarTasa(std::shared_ptr<TasaDeCambio> tasa);
  void eliminarMoneda(const std::string &uuid);
  void eliminarTasa(const std::string &uuid);

  // --- Herramientas de Debug (Modo Dios) ---
  void limpiarBaseDeDatos();
  void cargarDatosMock();

  // --- Lógica existente ---
  std::shared_ptr<Moneda> buscarMoneda(const std::string &siglas);
  std::shared_ptr<Moneda> buscarMonedaPorUuid(const std::string &uuid);
  void registrarMoneda(std::shared_ptr<Moneda> moneda);

  std::shared_ptr<TasaDeCambio> buscarTasa(const std::string &base,
                                           const std::string &destino);
  void establecerTasa(const std::string &siglasBase,
                      const std::string &siglasDestino, double valor);
  void establecerTasaPorUuid(const std::string &uuidBase,
                             const std::string &uuidDestino, double valor);

  double calcularValorImplicito(const std::string &base,
                                const std::string &destino);
  double convertirAXds(double monto, const std::string &siglasOrigen);
  bool tieneAmbiguedad(const std::string &base, const std::string &destino,
                       double valorManual);

  std::string getSiglasPivote() const;
  std::string getSiglasObjetivo() const;
  const std::vector<std::shared_ptr<Moneda>> &getMonedas() const;
  const std::vector<std::shared_ptr<TasaDeCambio>> &getTasas() const;
};
} // namespace Finexa

#endif
