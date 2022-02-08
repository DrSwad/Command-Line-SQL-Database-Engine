#pragma once

#include <string>
#include <vector>
#include <memory>

class Database;

enum class QueryType {
  SELECT,
  INSERT,
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
  std::vector<std::string> values;
  std::string condition;

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
  bool parse_select();
  bool parse_insert();

  bool execute_create_table();
  bool execute_drop_table();
  bool execute_select();
  bool execute_insert();
};