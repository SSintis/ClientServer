#pragma once

#include <sqlite3.h>
#include <string>

class dataBase{
public:
  dataBase(const std::string& filename);
  bool register_user(const std::string& login);
  bool auth_user(const std::string& login);

private:
  sqlite3* db;

};
