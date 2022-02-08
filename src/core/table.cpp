#include "core/table.h"

Table::Table(const std::string& table_name, const std::vector<std::string>& column_names)
  : name(table_name), columns(column_names) {}

Table::~Table() = default;

bool Table::insert_row(const std::vector<std::string>& values) {
  if (values.size() != columns.size()) {
    return false;
  }

  auto row = std::make_unique<Row>(columns, values);
  rows.push_back(std::move(row));

  return true;
}

std::vector<Row*> Table::select_rows() {
  std::vector<Row*> result;

  for (auto& row : rows) {
    result.push_back(row.get());
  }

  return result;
}