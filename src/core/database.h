#pragma once

#include <string>

class Database {
private:
  std::string name;
  bool is_open;

public:
  explicit Database(const std::string& db_name);
  ~Database();

  const std::string& get_name() const { return name; }
  bool is_open_db() const { return is_open; }
};