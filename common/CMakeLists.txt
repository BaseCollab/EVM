cmake_minimum_required(VERSION 3.13)

set(SOURCES 
    opcode_to_str.cpp
    str_to_opcode.cpp
    utils/string_operations.cpp
)

add_library(common_impl OBJECT ${SOURCES})
target_include_directories(common_impl PUBLIC ${EVM_ROOT})
