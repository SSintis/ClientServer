#include "../include/ChatClient.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

ChatClient::ChatClient(QObject* parent) : QObject(parent), is_running(false), sock(-1) {}
ChatClient::~ChatClient() { stop(); }

void ChatClient::connect_to_server(const std::string& ip, const int port, const Auth::AuthData& auth_data){
  this->auth_data = auth_data;
  
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

  // Отправить на сервер данные о подключившемся польователе через класс NetworkHandler

  is_running = true;
  receive_thread = std::thread(&ChatClient::received_thread, this); 
}
