#include "../include/client_command.hpp"
#include "../include/client_network.hpp"
#include "../include/client_ui.hpp"

#include <mutex>
#include <string>
#include <iostream>
#include <sys/socket.h>

command check_for_input_command(const std::string &message){
  std::string temp = message;
  if(temp[0] != '/') { return command::UNK; }
  temp.erase(0, 1);

  static const std::map<std::string, command> commands = {
    {"exit", command::EXIT},
    {"snu", command::SNU},
    {"help", command::HELP},
    {"history", command::HISTORY}
  };

  auto it = commands.find(temp);
  if(it != commands.end()) { return it->second; }
  else { return command::UNK; }
}

void handle_snu_command(
  int sock, nlohmann::json &message_packet, Auth::AuthData &userData,
  std::mutex& mtx, std::condition_variable& cv, bool& is_user_list_received){
  
  message_packet["command"] = "snu";
  message_packet["message"] = "request_users";

  std::string json_str = message_packet.dump();
  send_to_server(json_str, sock);

  {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&is_user_list_received] { return is_user_list_received; });
    is_user_list_received = false;
  }

  std::string new_user;
  std::cout << "Enter new receiver -> ";
  std::getline(std::cin, new_user);
  userData.receiver = new_user;

  message_packet["receiver"] = userData.receiver;
  message_packet["command"] = "";
}

void handle_history_command(int sock, nlohmann::json &message_packet){
  std::string user;
  std::cout << "Enter user -> ";
  std::getline(std::cin, user);

  message_packet["command"] = "history";
  message_packet["message"] = "request_history";
  message_packet["receiver"] = user;

  std::string json_str = message_packet.dump();
  send_to_server(json_str, sock);
}

void handle_help_command(){
  show_help();
}

void handle_exit_command(int sock, std::atomic<bool>& is_running){
  is_running = false;
  shutdown(sock, SHUT_RDWR);
}
