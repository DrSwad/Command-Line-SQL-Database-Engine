#include "core/database.h"

Database::Database(const std::string& db_name) : name(db_name), is_open(true) {}

Database::~Database() = default;