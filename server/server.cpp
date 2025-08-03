#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <unistd.h>

#include <iostream>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>

#include "dataBase.hpp"

std::unordered_map<std::string, int> client_sockets;

void handle_clients(int sock){
  while(true){
    uint32_t net_size;
    int size_read = recv(sock, &net_size, sizeof(net_size), 0);
    if(size_read <= 0) break;

    size_t data_size = ntohl(net_size);

    std::vector<char> buf(data_size);
    if(recv(sock, buf.data(), data_size, 0) <= 0) break;

    std::cout << "User " << sock << ": " << buf.data() << std::endl;
 
    try {
      auto json_data = nlohmann::json::parse(buf);
      if(json_data["command"] == "snu"){
        nlohmann::json message_packet;
        std::string users;

        for(const auto& pair : client_sockets){
          users += " " + pair.first;
        }
        message_packet["users"] = users;

        std::string json_str = message_packet.dump();
        uint32_t net_send_size = ntohl(json_str.size());
        send(sock, &net_size, sizeof(net_size), 0);
        send(sock, json_str.c_str(), json_str.size(), 0);

        continue;
      }
      std::string receiver = json_data["receiver"];

      if(client_sockets.find(receiver) == client_sockets.end()){
        std::cerr << "Unknown user - " << receiver << std::endl;
        continue;
      }

      uint32_t net_send_size = htonl(data_size);

      if(send(client_sockets[receiver], &net_send_size, sizeof(net_send_size), 0) < 0){
        std::cerr << "Failed to send size to " << receiver << std::endl;
      }
      if(send(client_sockets[receiver], buf.data(), data_size, 0) < 0){
        std::cerr << "Failed to send data to " << receiver << std::endl;
      }
    } catch (const nlohmann::json::parse_error& e) {
    
    }
  }
  close(sock);
}

void handle_auth(int sock){
  dataBase db("user_database.db");

  uint32_t net_size;
  if(recv(sock, &net_size, sizeof(net_size), 0) <= 0) return;

  size_t data_size = ntohl(net_size);

  std::vector<char> buf(data_size);
  if(recv(sock, buf.data(), data_size, 0) <= 0) return;

  std::string login;

  try {
    auto json_data = nlohmann::json::parse(buf);  
    login = json_data["user"]["sender"];

    if(!db.auth_user(login)) db.register_user(login);
    
    client_sockets[login] = sock;
    std::thread(handle_clients, sock).detach();
  
  } catch (nlohmann::json::parse_error& e) {
    std::cerr << "JSON error: " << e.what() << std::endl; 
    close(sock);
  }
}

int main(){
  int lisenear = socket(AF_INET, SOCK_STREAM, 0);
  int bytesRead;

  if(lisenear == -1){
    perror("Socket error");
    exit(1);
  }

  struct sockaddr_in addr;
  
  addr.sin_family = AF_INET;
  addr.sin_port = htons(3425);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(lisenear, (struct sockaddr *) &addr, sizeof addr) == -1){
    perror("bind error");
    exit(2);
  }

  int sock;
  listen(lisenear, 3);

  while(true){
    sock = accept(lisenear, NULL, NULL);
    if(sock == -1){
      perror("sock");
      exit(3); 
    }

    std::thread(handle_auth, sock).detach();
  }

  return 0;
}
