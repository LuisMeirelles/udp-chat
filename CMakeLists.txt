cmake_minimum_required(VERSION 4.1)
project(chat C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# library responsible by show warnings (microsoft and linux)
add_library(project_warnings INTERFACE)

if (MSVC)
    target_compile_options(project_warnings INTERFACE /W4)
else ()
    target_compile_options(project_warnings INTERFACE -Wall -Wextra -Wpedantic)
endif ()

# common library between server and client
add_library(chat_common STATIC
        src/chat/common.c
)
target_include_directories(chat_common PUBLIC include/chat)
target_link_libraries(chat_common PUBLIC project_warnings)

# library for server runtime
add_library(chat_server STATIC
        src/server/server_runtime.c
)
target_include_directories(chat_server PUBLIC include/server)
target_link_libraries(chat_server PUBLIC chat_common)

# linking main server with server runtime
add_executable(udp_server src/server/server.c)
target_link_libraries(udp_server PRIVATE chat_server)

# linking main client directly to common lib
add_executable(udp_client src/client/client.c)
target_link_libraries(udp_client PRIVATE chat_common)
