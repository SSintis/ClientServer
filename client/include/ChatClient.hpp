#ifndef CHAT_CLIENT_HPP
#define CHAT_CLIENT_HPP

#include <QObject>
#include <atomic>
#include <string>
#include <nlohmann/json.hpp>
#include <thread>

#include "../../common/Auth_Data.hpp"

class ChatClient : public QObject{
  Q_OBJECT
public:
  ChatClient(QObject* parent = nullptr);
  ~ChatClient();

  void connect_to_server(const std::string& ip, const int port, const Auth::AuthData& auth_data);
  void send_message(const std::string& message);
  void stop();

signals:
  void message_received(const QString& sender, const QString& message);
  void connection_error(const QString& error);

private:
  void received_thread();
  void proccesed_message(const nlohmann::json& data);

  int sock;
  std::atomic<bool> is_running;
  Auth::AuthData auth_data;
  std::thread receive_thread;
};

#endif
