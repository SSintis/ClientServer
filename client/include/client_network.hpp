#pragma once

#include <string>

namespace NetworkHandler {
  void send_to_server(const std::string& data, int sock);
  std::string receive_data(int sock);
}
