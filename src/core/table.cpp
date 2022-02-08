#include "core/table.h"
#include <sstream>

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

std::vector<Row*> Table::select_rows(const std::string& condition) {
  std::vector<Row*> result;

  for (auto& row : rows) {
    if (evaluate_condition(row.get(), condition)) {
      result.push_back(row.get());
    }
  }

  return result;
}

bool Table::evaluate_condition(Row* row, const std::string& condition) {
  std::istringstream iss(condition);
  std::string column, op, value;

  if (!(iss >> column >> op >> value)) {
    return true;
  }

  std::string row_value = row->get_value(column);

  if (op == "=") {
    return row_value == value;
  }
  else if (op == "!=") {
    return row_value != value;
  }
  else if (op == "<") {
    return row_value < value;
  }
  else if (op == ">") {
    return row_value > value;
  }
  else if (op == "<=") {
    return row_value <= value;
  }
  else if (op == ">=") {
    return row_value >= value;
  }

  return false;
}