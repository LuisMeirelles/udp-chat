cmake_minimum_required(VERSION 4.1)
project(chat C)

set(CMAKE_C_STANDARD 11)

add_executable(udp_server src/server.c)
add_executable(udp_client src/client.c)
