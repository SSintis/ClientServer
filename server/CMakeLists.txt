cmake_minimum_required(VERSION 3.16)
project(CMakeSDLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SQLite3 REQUIRED)

add_executable(server server.cpp
                      dataBase.cpp)

target_compile_features(server PRIVATE cxx_std_17)
target_link_libraries(server PRIVATE SQLite::SQLite3)

