#include "../include/client_network.hpp"
#include <cstdint>
#include <netinet/in.h>
#include <iostream>

void send_to_server(const std::string& data, int sock){
  uint32_t net_size = ntohl(data.size());
  if(send(sock, &net_size, sizeof(net_size), 0) < 0){
    std::cerr << "Error: Failed or there was some other problem sending the data size." << std::endl;
  }
  if(send(sock, data.data(), data.size(), 0) < 0){
    std::cerr << "Error: Failed or there were other problems sending data." << std::endl;
  }
}
