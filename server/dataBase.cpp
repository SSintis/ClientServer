#include "dataBase.hpp"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>

dataBase::dataBase(const std::string& filename){
  if(sqlite3_open(filename.c_str(), &db) != SQLITE_OK){
    std::cerr << "Error with open db: " << sqlite3_errmsg(db) << std::endl;
    exit(1);
  }
  
  const char* sql = "CREATE TABLE IF NOT EXISTS users ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "login TEXT UNIQUE NOT NULL);";

  if(sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK){
    std::cerr << "Error with create table: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    exit(1);
  }
}

bool dataBase::register_user(const std::string& login){
  sqlite3_stmt* stmt;
  const char* sql = "INSERT INTO users (login) VALUES (?);";

  if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK){
    std::cerr << "Ошибка при подготовке запроса(регистрация): " << sqlite3_errmsg(db) << std::endl;
    return false;
  }

  sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
  bool success = (sqlite3_step(stmt) == SQLITE_ROW);

  sqlite3_finalize(stmt);
  return success;
}

bool dataBase::auth_user(const std::string& login){
  sqlite3_stmt* stmt;
  const char* sql = "SELECT id FROM users WHERE login = ?;";

  if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK){
    std::cerr << "Ошибка при подготовке запроса: " << sqlite3_errmsg(db) << std::endl;
    return false;
  }

  sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
  bool found = (sqlite3_step(stmt) == SQLITE_ROW);

  sqlite3_finalize(stmt);
  return found;
}
