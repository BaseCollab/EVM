cmake_minimum_required(VERSION 3.13)

set(SOURCES 
    array.cpp
    string.cpp
)

add_library(memory_types_impl OBJECT ${SOURCES})
target_include_directories(memory_types_impl PUBLIC ${EVM_ROOT})
