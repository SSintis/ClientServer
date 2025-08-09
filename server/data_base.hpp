#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>

struct Message{
  std::string sender;
  std::string receiver;
  std::string message;
};

class dataBase{
public:
  dataBase(const std::string& filename);
  bool register_user(const std::string& login, const std::string& password_hash);
  bool auth_user(const std::string& login, const std::string& password_hash);

  bool save_message(Message message);
  std::vector<Message> get_message_history(const std::string& user1, const std::string& user2);

private:
  sqlite3* db;

};
