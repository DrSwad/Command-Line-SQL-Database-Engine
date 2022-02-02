#pragma once

#include <string>
#include <vector>

class Table {
private:
  std::string name;
  std::vector<std::string> columns;

public:
  Table(const std::string& table_name, const std::vector<std::string>& column_names);
  ~Table();

  const std::string& get_name() const { return name; }
  const std::vector<std::string>& get_columns() const { return columns; }
};