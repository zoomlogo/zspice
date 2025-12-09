cmake_minimum_required(VERSION 3.10)
project(zspice)

option(ASAN "Enable Debugging with Address Sanitizer." OFF)
if(ASAN)
    message(STATUS "Building with Address sanitizer...")
    add_compile_options(-g -fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(MAIN_SOURCES
    src/lu.c
    src/circuit.c
    src/error.c
    src/dc.c
)
set(COMPONENT_SOURCES
    src/component/component.c
    src/component/resistor.c
    src/component/voltage_source.c
)
set(TEST_SOURCES
    tests/test_lu.c
    tests/test_circuit.c
)

add_library(core ${COMPONENT_SOURCES} ${MAIN_SOURCES})
target_link_libraries(core PRIVATE m)

add_executable(zspice src/main.c)
target_link_libraries(zspice PRIVATE core)

add_executable(test_run tests/test.c ${TEST_SOURCES})
target_link_libraries(test_run PRIVATE core)
