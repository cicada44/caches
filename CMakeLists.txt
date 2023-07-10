cmake_minimum_required(VERSION 3.16)

project(
    "cache"
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(external)