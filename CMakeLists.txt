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
add_library(deps STATIC
        src/common.c
)
target_include_directories(deps PUBLIC include)
target_link_libraries(deps PUBLIC project_warnings)

# linking main client with client runtime
add_executable(udp-chat src/main.c)
target_link_libraries(udp-chat PRIVATE deps)
