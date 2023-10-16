cmake_minimum_required(VERSION 3.13)
                                                
set(ASM2BYTE_SOURCES asm2byte.cpp)

add_library(asm2byte_impl OBJECT ${ASM2BYTE_SOURCES})
target_include_directories(asm2byte_impl PUBLIC ${EVM_ROOT})

add_executable(asm2byte main.cpp)
target_link_libraries(asm2byte asm2byte_impl)
