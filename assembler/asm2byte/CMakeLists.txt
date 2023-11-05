cmake_minimum_required(VERSION 3.13)

add_executable(asm2byte main.cpp)
add_dependencies(asm2byte common_impl)
target_link_libraries(asm2byte PUBLIC common_impl)

add_library(asm2byte_static STATIC
    asm2byte.cpp
)

target_include_directories(asm2byte_static PUBLIC
    ${EVM_ROOT}
    ${EVM_BINARY_ROOT}
)
add_dependencies(asm2byte_static common_impl)
target_link_libraries(asm2byte_static PUBLIC common_impl)

target_link_libraries(asm2byte PUBLIC asm2byte_static)
