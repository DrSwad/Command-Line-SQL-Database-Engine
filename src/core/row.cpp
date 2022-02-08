#include "core/row.h"
#include <stdexcept>
#include <sstream>

Row::Row(const std::vector<std::string>& column_names, const std::vector<std::string>& row_values) {
  if (column_names.size() != row_values.size()) {
    throw std::invalid_argument("Column count doesn't match value count");
  }

  values = row_values;

  for (size_t i = 0; i < column_names.size(); ++i) {
    column_map[column_names[i]] = i;
  }
}

std::string Row::get_value(const std::string& column_name) const {
  auto it = column_map.find(column_name);

  if (it == column_map.end()) {
    throw std::out_of_range("Column not found: " + column_name);
  }
  return values[it->second];
}

std::string Row::to_string() const {
  std::ostringstream oss;

  for (size_t i = 0; i < values.size(); ++i) {
    if (i > 0) oss << " | ";
    oss << values[i];
  }
  return oss.str();
}