cmake_minimum_required(VERSION 4.1)
project(chat C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(project_warnings INTERFACE)

if (MSVC)
    target_compile_options(project_warnings INTERFACE /W4)
else ()
    target_compile_options(project_warnings INTERFACE -Wall -Wextra -Wpedantic)
endif ()

add_library(chat_common STATIC
        src/common.c
)

target_include_directories(chat_common PUBLIC include)

target_link_libraries(chat_common PUBLIC project_warnings)

add_executable(udp_server src/server.c)
add_executable(udp_client src/client.c)

target_link_libraries(udp_server PRIVATE chat_common)
target_link_libraries(udp_client PRIVATE chat_common)
