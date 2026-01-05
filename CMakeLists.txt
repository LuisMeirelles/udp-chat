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

# library for client runtime
add_library(chat_client STATIC
        src/client/client_runtime.c
        src/chat/common.c
        src/server/server_runtime.c
)
target_include_directories(chat_client PUBLIC include)
target_link_libraries(chat_client PUBLIC project_warnings)

# linking main client with client runtime
add_executable(udp_client src/client/client.c)
target_link_libraries(udp_client PRIVATE chat_client)
