#pragma once

#include <string>

namespace Auth {
struct AuthData{
  std::string login;
  std::string password;

  std::string receiver;
  // maybe ip-address
};

struct UserConnection {
  int sender_socket;
  int receiver_socket;
  std::string login;
};
}
