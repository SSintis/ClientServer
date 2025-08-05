#include "dataBase.hpp"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

dataBase::dataBase(const std::string& filename){
  if(sqlite3_open(filename.c_str(), &db) != SQLITE_OK){
    std::cerr << "Error with open db: " << sqlite3_errmsg(db) << std::endl;
    exit(1);
  }
  
  const char* sql_users = "CREATE TABLE IF NOT EXISTS users ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "login TEXT UNIQUE NOT NULL);";

  const char* sql_message = "CREATE TABLE IF NOT EXISTS messages ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "sender TEXT NOT NULL, "
                            "receiver TEXT NOT NULL, "
                            "message TEXT NOT NULL, "
                            "FOREIGN KEY(sender) REFERENCES users(login), "
                            "FOREIGN KEY(receiver) REFERENCES users(login));";

  if(sqlite3_exec(db, sql_users, NULL, NULL, NULL) != SQLITE_OK){
    std::cerr << "Error with create table: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    exit(1);
  }

  if(sqlite3_exec(db, sql_message, NULL, NULL, NULL) != SQLITE_OK){
    std::cerr << "Error with create table: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    exit(2);
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

bool dataBase::save_message(Message message){
  sqlite3_stmt* stmt;
  const char* sql = "INSERT INTO messages (sender, receiver, message) VALUES (?, ?, ?);";

  if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK){
    std::cerr << "Ошибка при подготовке запроса(запись сообщения): " << sqlite3_errmsg(db) << std::endl;
    return false;
  }

  sqlite3_bind_text(stmt, 1, message.sender.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, message.receiver.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, message.message.c_str(), -1, SQLITE_STATIC);
  bool success = (sqlite3_step(stmt) == SQLITE_ROW);

  sqlite3_finalize(stmt);
  return success;
}

std::vector<Message> dataBase::get_message_history(const std::string& user1, const std::string& user2){

}
