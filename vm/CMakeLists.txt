cmake_minimum_required(VERSION 3.13)

set(SOURCES vm.cpp)

add_library(vm_impl OBJECT ${SOURCES})
target_include_directories(vm_impl PUBLIC 
    ${EVM_ROOT}
    ${EVM_BINARY_ROOT}
)

target_link_libraries(vm_impl isa_gen_lib)
add_dependencies(vm_impl isa_gen_lib)
