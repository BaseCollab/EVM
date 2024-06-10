cmake_minimum_required(VERSION 3.13)

project(runtime)

add_compile_options(-Wno-invalid-offsetof)

set(SOURCES
    interpreter/interpreter.cpp
    memory/allocator/bump_allocator.cpp
    memory/allocator/freelist_allocator.cpp
    memory/garbage_collector/gc_stw.cpp
    memory/types/array.cpp
    memory/types/string.cpp
    memory/types/class.cpp
    memory/frame.cpp
    memory/heap_manager.cpp
    memory/class_manager.cpp
    runtime.cpp
)

add_library(runtime_impl OBJECT ${SOURCES})
target_include_directories(runtime_impl PUBLIC ${EVM_ROOT})
