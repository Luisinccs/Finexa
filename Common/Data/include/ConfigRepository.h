#pragma once

#include <string>

namespace Finexa::Data {

class ConfigRepository {
public:
  static std::string get(const std::string &key,
                         const std::string &defaultValue = "");
  static void set(const std::string &key, const std::string &value);
};

} // namespace Finexa::Data
