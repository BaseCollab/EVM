cmake_minimum_required(VERSION 3.13)

project(runtime)

set(SOURCES
    interpreter/interpreter.cpp
    memory/allocator/bump_allocator.cpp
    memory/types/array.cpp
    memory/types/string.cpp
    memory/frame.cpp
    memory/heap_manager.cpp
    runtime.cpp
)

add_library(runtime_impl OBJECT ${SOURCES})
target_include_directories(runtime_impl PUBLIC ${EVM_ROOT})
