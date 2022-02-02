#include "core/database.h"
#include <iostream>
#include <string>

int main() {
  std::cout << "=== Custom Database Engine ===" << std::endl;

  Database db("testdb");

  std::cout << "Goodbye!" << std::endl;

  return 0;
}