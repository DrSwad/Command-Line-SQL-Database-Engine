#pragma once

#include <string>
#include <vector>

class Database;

enum class QueryType {
  CREATE_TABLE,
  DROP_TABLE,
  UNKNOWN
};

class Query {
private:
  std::string sql;
  Database* database;
  QueryType type;
  std::string table_name;
  std::vector<std::string> columns;

public:
  Query(const std::string& sql_query, Database* db);

  bool parse();
  bool execute();
  std::string get_result() const;

private:
  std::vector<std::string> tokenize(const std::string& input);
  std::string to_lower(const std::string& input);

  bool parse_create_table();
  bool parse_drop_table();

  bool execute_create_table();
  bool execute_drop_table();
};