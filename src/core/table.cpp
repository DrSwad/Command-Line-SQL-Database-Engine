#include "core/table.h"
#include <sstream>
#include <algorithm>

Table::Table(const std::string& table_name, const std::vector<std::string>& column_names)
  : name(table_name), columns(column_names) {}

Table::~Table() = default;

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

bool Table::update_rows(const std::vector<std::string>& columns, const std::vector<std::string>& values, const std::string& condition) {
  for (auto& row : rows) {
    if (evaluate_condition(row.get(), condition)) {
      for (size_t i = 0; i < columns.size(); ++i) {
        row->set_value(columns[i], values[i]);
      }
    }
  }

  return true;
}

bool Table::delete_rows(const std::string& condition) {
  for (auto it = rows.begin(); it != rows.end();) {
    if (evaluate_condition(it->get(), condition)) {
      it = rows.erase(it);
    }
    else {
      ++it;
    }
  }

  return true;
}

bool Table::create_index(const std::string& column_name) {
  if (std::find(columns.begin(), columns.end(), column_name) == columns.end()) {
    return false;
  }

  if (secondary_indexes.find(column_name) != secondary_indexes.end()) {
    return false;
  }

  auto index = std::make_unique<BTree>();

  for (size_t i = 0; i < rows.size(); ++i) {
    std::string value = rows[i]->get_value(column_name);
    index->insert(value, i);
  }

  secondary_indexes[column_name] = std::move(index);
  return true;
}