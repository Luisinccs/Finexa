#include "../include/Helpers.hpp"
#include <cmath>

namespace Finexa {

double getValorDouble(int64_t rawValue, int decimalPlaces) {
  if (decimalPlaces <= 0)
    return static_cast<double>(rawValue);
  return static_cast<double>(rawValue) / std::pow(10, decimalPlaces);
}

int64_t getValorRaw(double value, int decimalPlaces) {
  if (decimalPlaces <= 0)
    return static_cast<int64_t>(value);
  return static_cast<int64_t>(std::round(value * std::pow(10, decimalPlaces)));
}

} // namespace Finexa
