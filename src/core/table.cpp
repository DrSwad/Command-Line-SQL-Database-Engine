#include "core/table.h"

Table::Table(const std::string& table_name, const std::vector<std::string>& column_names)
  : name(table_name), columns(column_names) {}

Table::~Table() = default;