cmake_minimum_required(VERSION 3.13)

set(SOURCES interpreter_test.cpp)

add_library(interpreter_test_obj OBJECT ${SOURCES})
target_include_directories(interpreter_test_obj PUBLIC ${EVM_ROOT})

add_dependencies(interpreter_test_obj asm2byte_static)
target_link_libraries(interpreter_test_obj PUBLIC asm2byte_static)
