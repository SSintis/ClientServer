#include "../include/client_network.hpp"
#include <cstdint>
#include <netinet/in.h>
#include <iostream>
#include <vector>

void NetworkHandler::send_to_server(const std::string& data, int sock){
  uint32_t net_size = ntohl(data.size());
  if(send(sock, &net_size, sizeof(net_size), 0) < 0){
    std::cerr << "Error: Failed or there was some other problem sending the data size." << std::endl;
  }
  if(send(sock, data.data(), data.size(), 0) < 0){
    std::cerr << "Error: Failed or there were other problems sending data." << std::endl;
  }
}

std::string NetworkHandler::receive_data(int sock){
  uint32_t net_size;
  if(recv(sock, &net_size, sizeof(net_size), 0) <= 0){
    return "Connection closed\0";
  }

  size_t data_size = ntohl(net_size);
  std::vector<char> buf(data_size + 1);
  buf[data_size] = '\0';

  if(recv(sock, buf.data(), data_size, 0) <= 0){
    return "Connection closed\0";
  } 
  return buf.data();
}
