#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

class Table;
class Query;

class Database {
private:
  std::unordered_map<std::string, std::unique_ptr<Table>> tables;
  std::string name;
  bool is_open;

public:
  explicit Database(const std::string& db_name);
  ~Database();

  const std::string& get_name() const { return name; }
  bool is_open_db() const { return is_open; }

  bool create_table(const std::string& table_name, const std::vector<std::string>& columns);
  bool drop_table(const std::string& table_name);
  Table* get_table(const std::string& table_name);
  const std::unordered_map<std::string, std::unique_ptr<Table>>& get_tables() const { return tables; }

  std::unique_ptr<Query> execute_query(const std::string& sql);
};