cmake_minimum_required(VERSION 3.13)

set(SOURCES interpreter.cpp)

add_library(interpreter_impl OBJECT ${SOURCES})
target_include_directories(interpreter_impl PUBLIC ${EVM_ROOT})

add_dependencies(interpreter_impl memory_types_impl)
target_link_libraries(interpreter_impl PUBLIC memory_types_impl)
