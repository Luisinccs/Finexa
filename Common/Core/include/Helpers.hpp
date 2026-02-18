#ifndef FINEXA_HELPERS_HPP
#define FINEXA_HELPERS_HPP

#include <cstdint>

namespace Finexa {

/**
 * @brief Convierte un valor entero "raw" (con decimales implícitos) a double.
 * @param rawValue El valor entero (ej: 52000)
 * @param decimalPlaces Número de decimales (ej: 2)
 * @return El valor real (ej: 520.00)
 */
double getValorDouble(int64_t rawValue, int decimalPlaces);

/**
 * @brief Convierte un double a su representación entera "raw" según los
 * decimales deseados.
 * @param value El valor real (ej: 520.00)
 * @param decimalPlaces Número de decimales (ej: 2)
 * @return El valor entero (ej: 52000)
 */
int64_t getValorRaw(double value, int decimalPlaces);

} // namespace Finexa

#endif // FINEXA_HELPERS_HPP
