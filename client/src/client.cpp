#include <condition_variable>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <ostream>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>
#include <atomic>

#include "../../common/Auth_Data.hpp"

#include "../include/client_network.hpp"
#include "../include/client_command.hpp"

std::atomic<bool> is_running{true};

std::mutex mtx;
std::condition_variable cv;
bool is_user_list_received = false;

void send_message(int sock, Auth::AuthData userData){
  std::string message;
  nlohmann::json message_packet; 

  message_packet["user"] = {
    {"sender", userData.login},
    {"password", userData.password}
  };
  message_packet["receiver"] = userData.receiver;

  while(is_running){
    message_packet["command"] = "";
    std::getline(std::cin, message);
    
    command returned_command = check_for_input_command(message);
    switch (returned_command) {
      case command::EXIT: {
        handle_exit_command(sock, is_running);
        return;
      }
      case command::SNU:{
        handle_snu_command(sock, message_packet, userData, mtx, cv, is_user_list_received);
        message = "";
        break;
      }
      case command::HISTORY:{
        handle_history_command(sock, message_packet);
        continue;
      }
      case command::HELP:{
        handle_help_command();
        continue;
      }
      case command::UNK: break;
    }
    message_packet["message"] = message;

    std::string json_str = message_packet.dump();
    send_to_server(json_str, sock);
  }
}

void receives_message(int sock){
  while(is_running){
    uint32_t net_size;
    if(recv(sock, &net_size, sizeof(net_size), 0) <= 0 || !is_running){
      std::cout << "Connection closed" << std::endl;
      is_running = false;
      return;
    }
    
    size_t data_size = ntohl(net_size);
    std::vector<char> buf(data_size + 1);
    buf[data_size] = '\0';

    if(recv(sock, buf.data(), data_size, 0) <= 0){
      std::cout << "Server turn off" << std::endl;
      is_running = false;
      return;
    }
  
    try {
      auto json_data = nlohmann::json::parse(buf);
      
      if(json_data["type"] == "user_list"){
        std::cout << json_data["users"];
        {
          std::lock_guard<std::mutex> lock(mtx);
          is_user_list_received = true;
        }
        cv.notify_one();
      }
      else if(json_data["type"] == "message"){
        std::cout << json_data["user"]["sender"] << ": " << json_data["message"] << std::endl;
      }
      else if(json_data["type"] == "history"){
        system("clear");
        std::string his = json_data["history"];
        std::cout << his.c_str() << std::endl;
      }
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
  addr.sin_addr.s_addr = inet_addr("192.168.100.11");

  if(connect(sock, (struct sockaddr *) &addr, sizeof addr) == -1){
    perror("connect");
    exit(2);
  }
  
  send_to_server(auth_json, sock);

  std::thread receiver(receives_message, sock);
  send_message(sock, newUser);

  if(!is_running){
    receiver.join();
    close(sock);
    return 0;
  }
}
