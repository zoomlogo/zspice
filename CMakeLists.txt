cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(spice)

include_directories(src)

add_executable(spice src/main.c src/lu.c)
target_link_libraries(spice)
