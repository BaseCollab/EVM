cmake_minimum_required(VERSION 3.13)

set(SOURCES 
    frame.cpp
    heap_manager.cpp
)

add_library(frame_impl OBJECT ${SOURCES})
target_include_directories(frame_impl PUBLIC ${EVM_ROOT})

add_subdirectory(allocator)
add_subdirectory(types)
