add_custom_target(generate_opcode_map
    COMMAND python3 ${EVM_ROOT}/isa/generate.py ${EVM_ROOT}/isa/isa.yaml
)
