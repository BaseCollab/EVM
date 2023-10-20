cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(EVM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EVM_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(EVM_BINARY_ROOT ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Werror -Wshadow")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${EVM_BINARY_ROOT}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${EVM_BINARY_ROOT}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EVM_BINARY_ROOT}/bin)

add_subdirectory(isa)
add_subdirectory(assembler)
add_subdirectory(interpreter)
add_subdirectory(vm)

add_library(evm_static STATIC)
target_link_libraries(evm_static PUBLIC
    interpreter_impl
    vm_impl
)

# --------------------------clang-format--------------------------------------

file(GLOB_RECURSE ALL_SOURCES_FILES *.cpp *.h)

find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format-14 clang-format)

add_custom_target(clang-force-format
    COMMAND ${CLANG_FORMAT_EXECUTABLE}
    -i
    -style=file
    --verbose
    ${ALL_SOURCES_FILES}
)

# --------------------------google-tests--------------------------------------

enable_testing()
find_package(GTest REQUIRED)

add_executable(unit_tests)

target_link_libraries(unit_tests PUBLIC evm_static GTest::gtest_main)

add_subdirectory(tests)

# ----------------------------------------------------------------------------

add_executable(evm vm/evm.cpp)
target_include_directories(evm PUBLIC ${EVM_ROOT})
target_link_libraries(evm PUBLIC evm_static)
