cmake_minimum_required(VERSION 3.13)

set(SOURCES 
    bump_allocator_test.cpp
    freelist_allocator_test.cpp
)

add_library(memory_tests_obj OBJECT ${SOURCES})
target_include_directories(memory_tests_obj PUBLIC ${EVM_ROOT})

add_executable(memory_tests ${EVM_ROOT}/tests/main.cpp)
target_link_libraries(memory_tests PUBLIC memory_tests_obj GTest::gtest_main)

target_link_libraries(memory_tests PUBLIC runtime_impl)
add_dependencies(memory_tests runtime_impl)

add_custom_target(run_memory_tests
    COMMENT "Running memory tests"
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/memory_tests --gtest_color=yes
)
add_dependencies(run_memory_tests memory_tests)
