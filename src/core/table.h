#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "storage/btree.h"
#include "core/row.h"

class Table {
private:
  std::string name;
  std::vector<std::string> columns;
  std::vector<std::unique_ptr<Row>> rows;
  std::unordered_map<std::string, std::unique_ptr<BTree>> secondary_indexes;

public:
  Table(const std::string& table_name, const std::vector<std::string>& column_names);
  ~Table();

  const std::string& get_name() const { return name; }
  const std::vector<std::string>& get_columns() const { return columns; }
  size_t get_row_count() const { return rows.size(); }

  bool insert_row(const std::vector<std::string>& values);
  std::vector<Row*> select_rows(const std::string& condition = "");
  bool update_rows(const std::vector<std::string>& columns, const std::vector<std::string>& values, const std::string& condition);
  bool delete_rows(const std::string& condition);

  bool create_index(const std::string& column_name);

private:
  bool evaluate_condition(Row* row, const std::string& condition);
};