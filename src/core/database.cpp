#include "core/database.h"
#include "core/table.h"
#include "query/query.h"

Database::Database(const std::string& db_name) : name(db_name), is_open(true) {}

Database::~Database() = default;

bool Database::create_table(const std::string& table_name, const std::vector<std::string>& columns) {
  if (tables.find(table_name) != tables.end()) {
    return false;
  }
  tables[table_name] = std::make_unique<Table>(table_name, columns);
  return true;
}

bool Database::drop_table(const std::string& table_name) {
  return tables.erase(table_name) > 0;
}

Table* Database::get_table(const std::string& table_name) {
  auto it = tables.find(table_name);

  return (it != tables.end()) ? it->second.get() : nullptr;
}

std::unique_ptr<Query> Database::execute_query(const std::string& sql) {
  return std::make_unique<Query>(sql, this);
}