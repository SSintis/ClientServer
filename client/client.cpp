#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <nlohmann/json_fwd.hpp>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <unistd.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

#include "../common/AuthData.hpp"

void send_message(int sock, Auth::AuthData userData){
  char message[1024];
  nlohmann::json message_packet; 

  message_packet["user"] = {
    {"sender", userData.login},
    {"password", userData.password}
  };
  message_packet["receiver"] = userData.receiver;

  while(true){
    std::cin.getline(message, sizeof(message));
    message_packet["message"] = message;

    std::string json_str = message_packet.dump();
    uint32_t net_size = ntohl(json_str.size());
    send(sock, &net_size, sizeof(net_size), 0);
    send(sock, json_str.c_str(), json_str.size(), 0);
  }
}

void receives_message(int sock){
  while(true){
    uint32_t net_size;
    if(recv(sock, &net_size, sizeof(net_size), 0) <= 0){
      std::cout << "Server turn off" << std::endl;
      exit(1);
    }
    
    size_t data_size = ntohl(net_size);
    std::vector<char> buf(data_size);

    if(recv(sock, buf.data(), data_size, 0) <= 0){
      std::cout << "Server turn off" << std::endl;
      exit(2);
    }
  
    try {
      auto json_data = nlohmann::json::parse(buf);
      std::cout << json_data["user"]["sender"] << ": " << json_data["message"] << std::endl;
    } catch (const nlohmann::json::parse_error& e) {
      std::cerr << "JSON error: " << e.what() << std::endl;
    }
  }
}

int main(){
  // -----------------------------------------

  Auth::AuthData newUser;
  
  std::cout << "Enter login -> ";
  std::getline(std::cin, newUser.login);

  std::cout << "Enter password (you can write 0) -> ";
  std::getline(std::cin, newUser.password);

  std::cout << "Enter recipient -> ";
  std::getline(std::cin, newUser.receiver);

  nlohmann::json auth_data;
  auth_data["user"] = {
    {"sender", newUser.login},
    {"password", newUser.password}
  };
 
  auth_data["receiver"] = newUser.receiver;
  std::string auth_json = auth_data.dump();
  // -------------------------------------------

  struct sockaddr_in addr;
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  if(sock == -1){
    perror("socket");
    exit(1);
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(3425);
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  if(connect(sock, (struct sockaddr *) &addr, sizeof addr) == -1){
    perror("connect");
    exit(2);
  }
  
  uint32_t net_size = htonl(auth_json.size());
  send(sock, &net_size, sizeof(net_size), 0);
  send(sock, auth_json.c_str(), auth_json.size(), 0);

  std::thread receiver(receives_message, sock);
  receiver.detach();

  send_message(sock, newUser);

  close(sock);
}
