cmake_minimum_required(VERSION 3.13)

set(SOURCES bump_allocator_test.cpp)

add_library(memory_tests_impl OBJECT ${SOURCES})
target_include_directories(memory_tests_impl PUBLIC ${EVM_ROOT})

target_link_libraries(unit_tests PUBLIC memory_tests_impl)
add_dependencies(run_unit_tests unit_tests)
