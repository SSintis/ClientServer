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
#include <vector>
#include <thread>
#include <atomic>

#include "../common/AuthData.hpp"

std::atomic<bool> is_running{true};

std::mutex mtx;
std::condition_variable cv;
bool is_user_list_received = false;

enum class command{
  EXIT,       
  SNU,          
  HELP,         
  UNK           
};

void help(){
  std::cout << "\033[1;36m--- HELP ---\033[0m\n";
  std::cout << "/exit - завершить программу\n";
  std::cout << "/snu - сменить получателя сообщений\n";
  std::cout << "/help - вывести это сообщение\n";
}

command check_for_input_command(const std::string& message){
  std::string temp = message;
  if(temp[0] != '/'){ return command::UNK; }
  temp.erase(0, 1);

  static const std::map<std::string, command> commands = {
    {"exit", command::EXIT},
    {"snu", command::SNU},
    {"help", command::HELP}
  };

  auto it = commands.find(temp);
  if(it != commands.end()) { return it->second; }
  else { return command::UNK; }
}

void send_message(int sock, Auth::AuthData userData){
  std::string message;
  nlohmann::json message_packet; 

  message_packet["user"] = {
    {"sender", userData.login},
    {"password", userData.password}
  };
  message_packet["receiver"] = userData.receiver;
  message_packet["command"] = "";

  while(is_running){
    std::getline(std::cin, message);
    
    command returned_command = check_for_input_command(message);
    switch (returned_command) {
      case command::EXIT: {
        is_running = false;
        shutdown(sock, SHUT_RDWR);
        return;
      }
      case command::SNU:{
        message_packet["command"] = "snu";
        message_packet["message"] = "request_users";

        std::string json_str = message_packet.dump();
        uint32_t net_size = ntohl(json_str.size());
        send(sock, &net_size, sizeof(net_size), 0);
        send(sock, json_str.data(), json_str.size(), 0);

        {
          std::unique_lock<std::mutex> lock(mtx);
          cv.wait(lock, [] { return is_user_list_received; });
          is_user_list_received = false;
        }

        std::string new_user;
        std::cout << "Enter new login -> ";
        std::getline(std::cin, new_user);
        userData.receiver = new_user;

        message_packet["receiver"] = userData.receiver;
        message = "NEW CONNECTION!!!";
        message_packet["command"] = "\0";
        break;
      }
      case command::HELP:{
        help();
        continue;
      }
      case command::UNK: break;
    }
    message_packet["message"] = message;

    std::string json_str = message_packet.dump();
    uint32_t net_size = ntohl(json_str.size());
    send(sock, &net_size, sizeof(net_size), 0);
    send(sock, json_str.c_str(), json_str.size(), 0);
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
  
  uint32_t net_size = htonl(auth_json.size());
  send(sock, &net_size, sizeof(net_size), 0);
  send(sock, auth_json.c_str(), auth_json.size(), 0);

  std::thread receiver(receives_message, sock);
  send_message(sock, newUser);

  if(!is_running){
    receiver.join();
    close(sock);
    return 0;
  }
}
