cmake_minimum_required(VERSION 3.10)

option(ASAN "Enable Debugging with Address Sanitizer." OFF)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(spice)

include(CTest)

include_directories(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(spice src/main.c src/lu.c src/node.c src/circuit.c src/component/component.c src/map.c)

if (ASAN)
    message(STATUS "Building with Address sanitizer...")
    target_compile_options(spice PRIVATE -g -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(spice PRIVATE -fsanitize=address)
endif()

target_link_libraries(spice)

add_executable(test_lu tests/test_lu.c src/lu.c)
add_test(NAME test_lu COMMAND test_lu)
