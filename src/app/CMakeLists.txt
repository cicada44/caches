set(target_name app)
set(target_name_iC idealCache)

add_executable(
    ${target_name}
)

add_executable(
    ${target_name_iC}
)

target_sources(
    ${target_name_iC}
    PRIVATE
        idealCache.cpp
)

target_sources(
    ${target_name}
    PRIVATE
        main.cpp
)

include(CompileOptions)
set_compile_options(${target_name})
set_compile_options(${target_name_iC})

target_include_directories(
    ${target_name}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_include_directories(
    ${target_name_iC}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)
