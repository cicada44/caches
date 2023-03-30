cmake_minimum_required(VERSION 3.22)

project(
    "cache"
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_program(
    CLANG_TIDY_EXE
    NAMES
        clang-tidy
        clang-tidy-12
)

if (NOT CLANG_TIDY_EXE)
    message("[E] clang-tidy not found")
else()
    execute_process(
        COMMAND ${CLANG_TIDY_EXE} --version
        OUTPUT_VARIABLE CLANG_TIDY_VERSION
    )
    message("clang-tidy found:\n" ${CLANG_TIDY_VERSION})
endif()


add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(external)