cmake_minimum_required(VERSION 3.16)
project(CMakeMESSANGERProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)  # Включено -> .ui файл можно не указывать

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(client
  main.cpp
  mainwindow.cpp
  LoginWindow.cpp 
  # mainwindow.ui  # Необязательно, но можно добавить для ясности
)

target_link_libraries(client
  Qt6::Core
  Qt6::Widgets
)
