#pragma once

#include <string>
#include <vector>
#include <memory>
#include "core/row.h"

class Table {
private:
  std::string name;
  std::vector<std::string> columns;
  std::vector<std::unique_ptr<Row>> rows;

public:
  Table(const std::string& table_name, const std::vector<std::string>& column_names);
  ~Table();

  const std::string& get_name() const { return name; }
  const std::vector<std::string>& get_columns() const { return columns; }

  bool insert_row(const std::vector<std::string>& values);
  std::vector<Row*> select_rows();
  size_t get_row_count() const { return rows.size(); }
};