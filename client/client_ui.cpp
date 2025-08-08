#include "client_ui.hpp"
#include <iostream>

void show_help(){
  std::cout << "\033[1;36m--- HELP ---\033[0m\n";
  std::cout << "/exit    - завершить программу.\n";
  std::cout << "/snu     - сменить получателся сообщений.\n";
  std::cout << "/history - вывести историю сообщений.\n";
  std::cout << "/help    - вывести это сообщение.";
}
