cmake_minimum_required(VERSION 3.16)
project(CMakeMESSANGERProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)  # Включение автоматической обработки Q_OBJECT
set(CMAKE_AUTOUIC ON)

# Явно укажите заголовочные файлы для moc
file(GLOB HEADERS include/*.hpp ../common/*.hpp)

add_executable(client 
    src/ChatClient.cpp
    src/client_network.cpp
    ui/LoginWindow.cpp
    ui/mainwindow.cpp
    main.cpp
    ${HEADERS}  # Добавляем заголовки для moc
)

# Укажите пути к заголовкам
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/../common
)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)

target_link_libraries(client
    Qt6::Core
    Qt6::Network  # Добавлено для работы с сокетами
    Qt6::Widgets
)

target_compile_features(client PRIVATE cxx_std_17)
