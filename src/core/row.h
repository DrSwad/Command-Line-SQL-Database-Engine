#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class Row {
private:
  std::vector<std::string> values;

public:
  Row(const std::vector<std::string>& column_names, const std::vector<std::string>& row_values);

  std::string to_string() const;
};