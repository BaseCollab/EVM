find_package(Python3 REQUIRED)

set(ISA_SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/generated/opcode.h
    ${CMAKE_CURRENT_BINARY_DIR}/generated/opcode_name_map.h
    ${CMAKE_CURRENT_BINARY_DIR}/generated/opcode_name_map.cpp
)

add_custom_command(OUTPUT ${ISA_SOURCES} 
    COMMAND ${CMAKE_COMMAND} -E make_directory generated 
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/generate.py ${CMAKE_CURRENT_SOURCE_DIR}/isa.yaml
)

add_custom_target(generate_isa_sources DEPENDS ${ISA_SOURCES})

add_library(isa_gen_lib ${ISA_SOURCES})
add_dependencies(isa_gen_lib generate_isa_sources)
