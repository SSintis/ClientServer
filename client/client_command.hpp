#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <nlohmann/json.hpp>
#include "../common/Auth_Data.hpp"

enum class command{
  EXIT,
  SNU,
  HELP,
  HISTORY,
  UNK
};

void handle_snu_command(int sock, nlohmann::json& message_packet, Auth::AuthData& userData,
                        std::mutex& mtx, std::condition_variable& cv, bool& is_user_list_received);
void handle_history_command(int sock, nlohmann::json& message_packet);
void handle_help_command();
void handle_exit_command(int sock, std::atomic<bool>& is_running);

command check_for_input_command(const std::string& message);
