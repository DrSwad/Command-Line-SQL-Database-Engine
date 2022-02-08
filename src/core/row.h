#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class Row {
private:
  std::vector<std::string> values;
  std::unordered_map<std::string, size_t> column_map;

public:
  Row(const std::vector<std::string>& column_names, const std::vector<std::string>& row_values);

  std::string get_value(const std::string& column_name) const;

  std::string to_string() const;
};