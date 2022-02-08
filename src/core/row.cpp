#include "core/row.h"
#include <stdexcept>
#include <sstream>

Row::Row(const std::vector<std::string>& column_names, const std::vector<std::string>& row_values) {
  if (column_names.size() != row_values.size()) {
    throw std::invalid_argument("Column count doesn't match value count");
  }

  values = row_values;
}

std::string Row::to_string() const {
  std::ostringstream oss;

  for (size_t i = 0; i < values.size(); ++i) {
    if (i > 0) oss << " | ";
    oss << values[i];
  }
  return oss.str();
}