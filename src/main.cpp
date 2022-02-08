#include "core/database.h"
#include "core/table.h"
#include "query/query.h"
#include <iostream>
#include <string>

int main() {
  std::cout << "=== Custom Database Engine ===" << std::endl;
  std::cout << "Type 'help' for commands, 'quit' to exit" << std::endl;

  Database db("testdb");

  std::string input;

  while (true) {
    std::cout << "\ndb> ";
    std::getline(std::cin, input);

    if (input == "quit" || input == "exit" || input == "") {
      break;
    }

    if (input == "help") {
      std::cout << "Available commands:" << std::endl;
      std::cout << "  CREATE TABLE table_name (col1, col2, ...)" << std::endl;
      std::cout << "  DROP TABLE table_name" << std::endl;
      std::cout << "  TABLES" << std::endl;
      std::cout << "  INSERT INTO table_name (col1, col2, ...) VALUES (val1, val2, ...)" << std::endl;
      std::cout << "  SELECT * FROM table_name" << std::endl;
      std::cout << "  quit" << std::endl;
      continue;
    }

    if (input == "TABLES") {
      const auto& tables = db.get_tables();

      if (tables.empty()) {
        std::cout << "No tables in database" << std::endl;
      }
      else {
        std::cout << "Tables in database:" << std::endl;

        for (const auto& table_pair : tables) {
          std::cout << "  " << table_pair.first << " ("
                    << table_pair.second->get_row_count() << " rows)" << std::endl;
        }
      }
      continue;
    }

    auto query = db.execute_query(input);

    if (query->parse()) {
      if (query->execute()) {
        std::cout << "Query executed successfully" << std::endl;
      }
      else {
        std::cout << "Query execution failed" << std::endl;
      }
    }
    else {
      std::cout << "Invalid SQL syntax" << std::endl;
    }
  }

  std::cout << "Goodbye!" << std::endl;

  return 0;
}