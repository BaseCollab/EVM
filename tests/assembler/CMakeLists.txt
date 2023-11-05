cmake_minimum_required(VERSION 3.13)

set(SOURCES asm2byte_test.cpp)

add_library(assembler_test_obj OBJECT ${SOURCES})
target_include_directories(assembler_test_obj PUBLIC ${EVM_ROOT})

add_dependencies(assembler_test_obj asm2byte_static)
target_link_libraries(assembler_test_obj PUBLIC asm2byte_static)
