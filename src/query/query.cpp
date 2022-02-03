#include "query/query.h"
#include "core/database.h"
#include <algorithm>
#include <sstream>

Query::Query(const std::string& sql_query, Database* db)
  : sql(sql_query), database(db), type(QueryType::UNKNOWN) {}

std::vector<std::string> Query::tokenize(const std::string& input) {
  std::vector<std::string> tokens;
  std::istringstream iss(input);
  std::string token;

  while (iss >> token) {
    if (token.back() == ';') {
      token.pop_back();
    }

    if (token.front() == '(') {
      tokens.push_back(token.substr(0, 1));
      token.erase(0, 1);
    }

    if (!token.empty()) {
      if (token.back() == ')' || token.back() == ',') {
        tokens.push_back(token.substr(0, token.size() - 1));
        tokens.push_back(token.substr(token.size() - 1, 1));
      }
      else {
        tokens.push_back(token);
      }
    }
  }

  return tokens;
}

std::string Query::to_lower(const std::string& input) {
  std::string result = input;

  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result;
}

bool Query::parse() {
  std::vector<std::string> tokens = tokenize(sql);

  if (tokens.empty()) return false;

  std::string first_token = to_lower(tokens[0]);

  if (first_token == "create" && tokens.size() > 1 && to_lower(tokens[1]) == "table") {
    type = QueryType::CREATE_TABLE;
    return parse_create_table();
  }
  else if (first_token == "drop" && tokens.size() > 1 && to_lower(tokens[1]) == "table") {
    type = QueryType::DROP_TABLE;
    return parse_drop_table();
  }

  return false;
}

bool Query::execute() {
  switch (type) {
    case QueryType::CREATE_TABLE: {
      return execute_create_table();
    }
    case QueryType::DROP_TABLE: {
      return execute_drop_table();
    }
    default: {
      return false;
    }
  }
}

bool Query::parse_create_table() {
  std::vector<std::string> tokens = tokenize(sql);
  size_t i = 2;

  if (i >= tokens.size()) return false;
  table_name = tokens[i];
  i++;

  if (i >= tokens.size() || tokens[i] != "(") return false;
  i++;

  while (i < tokens.size() && tokens[i] != ")") {
    if (tokens[i] != ",") {
      columns.push_back(tokens[i]);
    }
    i++;
  }

  return true;
}

bool Query::parse_drop_table() {
  std::vector<std::string> tokens = tokenize(sql);
  size_t i = 2;

  if (i >= tokens.size()) return false;
  table_name = tokens[i];

  return true;
}

bool Query::execute_create_table() {
  return database->create_table(table_name, columns);
}

bool Query::execute_drop_table() {
  return database->drop_table(table_name);
}

std::string Query::get_result() const {
  return "Query executed successfully";
}