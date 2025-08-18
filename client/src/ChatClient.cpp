#include "../include/ChatClient.hpp"
#include "../include/client_network.hpp"
#include "qglobal.h"
#include "qobject.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

ChatClient::ChatClient(QObject* parent) : QObject(parent), is_running(false), sock(-1) {}
ChatClient::~ChatClient() { stop(); }

void ChatClient::connect_to_server(const std::string& ip, const int port, const Auth::AuthData& auth_data){
  this->user_data = auth_data;
  
  struct sockaddr_in addr;
  sock = socket(AF_INET, SOCK_STREAM, 0);

  if(sock == -1) {
    emit connection_error("Failed to create socket");
    return;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip.c_str());

  if(::connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){
    emit connection_error("Failed to connect to server");
    close(sock);
    return;
  }
 
  nlohmann::json user_data_packet;
  user_data_packet["user"] = {
    {"sender", this->user_data.login},
    {"password", this->user_data.password}
  };
  user_data_packet["receiver"] = this->user_data.receiver;

  NetworkHandler::send_to_server(user_data_packet.dump(), sock);

  is_running = true;
  receive_thread = std::thread(&ChatClient::received_thread, this); 
}

void ChatClient::send_message(const std::string& message){
  nlohmann::json message_packet;
  message_packet["user"] = {
    {"sender", user_data.login},
    {"password", user_data.password}
  };
  message_packet["receiver"] = user_data.receiver;
  message_packet["message"] = message;
  message_packet["command"] = "";
  message_packet["type"] = "message";

  NetworkHandler::send_to_server(message_packet.dump(), this->sock);
}

void ChatClient::stop(){
  if(!is_running) return;
  
  is_running = false;
  shutdown(sock, SHUT_RDWR);

  if(receive_thread.joinable()){
    receive_thread.join();
  }

  close(sock);
  this->sock = -1;
}

void ChatClient::received_thread(){
  while(is_running){
    auto data = NetworkHandler::receive_data(this->sock); 
    if(!data.empty()){
      try {
        auto json_data = nlohmann::json::parse(data);
        proccesed_message(json_data);
      } catch (nlohmann::json::parse_error& e) {
      
      }
    } 
  }
}

void ChatClient::proccesed_message(const nlohmann::json& data){
  if(data["type"] == "message"){
    QString sender = QString::fromStdString(data["user"]["sender"]);
    QString message = QString::fromStdString(data["message"]);

    emit message_received(sender, message);
  }
}
