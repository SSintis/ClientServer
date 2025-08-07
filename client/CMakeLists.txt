cmake_minimum_required(VERSION 3.16)
project(CMakeMESSANGERProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(client client.cpp
                      client_network.cpp)

target_compile_features(client PRIVATE cxx_std_17)
