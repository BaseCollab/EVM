cmake_minimum_required(VERSION 3.13)

set(SOURCES 
    runtime.cpp
    class_linker.cpp
)

add_library(runtime_impl OBJECT ${SOURCES})
target_include_directories(runtime_impl PUBLIC ${EVM_ROOT})
