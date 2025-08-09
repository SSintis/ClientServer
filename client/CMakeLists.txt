cmake_minimum_required(VERSION 3.16)
project(CMakeMESSANGERProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(client src/client.cpp
                      src/client_network.cpp
                      src/client_command.cpp
                      src/client_ui.cpp)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)

target_link_libraries(client
                      Qt6::Core
                      Qt6::Network
                      Qt6::Widgets)

target_compile_features(client PRIVATE cxx_std_17)
