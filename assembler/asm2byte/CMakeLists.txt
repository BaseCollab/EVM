cmake_minimum_required(VERSION 3.13)
               
add_executable(asm2byte main.cpp)

add_library(asm2byte_impl OBJECT
    asm2byte.cpp
)

target_include_directories(asm2byte_impl PUBLIC
    ${EVM_ROOT}
    ${EVM_BINARY_ROOT}
)
target_link_libraries(asm2byte_impl isa_gen_lib)

target_link_libraries(asm2byte asm2byte_impl)
