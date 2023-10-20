import yaml
import sys

def parse_yaml(filename):
    with open(filename, 'r') as instr_file:
        return yaml.safe_load(instr_file)

def generate_name_opcode_switch_case(yaml_dict):
    with open('generated/opcode_name_map.h', 'w') as fout:
        fout.write("#ifndef EVM_ISA_OPCODE_NAME_MAP_H\n"
                   "#define EVM_ISA_OPCODE_NAME_MAP_H\n\n"
                   "#include <string_view>\n"
                   "#include <unordered_map>\n\n"
                   "#include \"opcode.h\"\n\n"
                   "namespace evm::isa {\n\n")

        fout.write("Opcode GetOpcodeByName(const std::string_view &insn_name);\n\n")
        fout.write('static std::unordered_map<std::string_view, Opcode> NAME_OPCODE_MAP = {\n')

        for key in yaml_dict:
            fout.write(f"    {{\"{key}\", Opcode::{key.upper()}}},\n")

        fout.write("};\n\n")

        fout.write("} // namespace evm::isa\n\n")

        fout.write("#endif // EVM_ISA_OPCODE_NAME_MAP_H\n")
    return

def generate_name_to_opcode():
    with open('generated/opcode_name_map.cpp', 'w') as fout:
        fout.write(
            "#include \"opcode_name_map.h\"\n\n"
            "namespace evm::isa {\n\n"
            "Opcode GetOpcodeByName(const std::string_view &insn_name)\n"
            "{\n"
            "    auto it = NAME_OPCODE_MAP.find(insn_name);\n"
            "    if (it != NAME_OPCODE_MAP.end()) {\n"
            "        return it->second;\n"
            "    }\n"
            "    return Opcode::INVALID;\n"
            "}\n\n"
            "} // namespace evm::isa\n"
        )
    return

def generate_opcode(yaml_dict):
    with open('generated/opcode.h', 'w') as fout:
        fout.write("#ifndef EVM_ISA_OPCODES_ISA_OPCODES_H\n"
                   "#define EVM_ISA_OPCODES_ISA_OPCODES_H\n\n"
                   "namespace evm::isa {\n\n")

        fout.write('enum class Opcode {\n')

        for key in yaml_dict:
            opcode = (yaml_dict[key])['opcode']
            fout.write(f"    {key.upper()} = {hex(opcode)},\n")

        fout.write("};\n\n")

        fout.write("} // namespace evm::isa\n\n")

        fout.write("#endif // EVM_ISA_OPCODES_ISA_OPCODES_H\n")
    return

def main():
    if len(sys.argv) != 2:
        print('Invalid amount of arguments [1 must be used]',        file=sys.stderr)
        print('Usage example: ' + sys.argv[0] + ' ' + 'isa.yaml', file=sys.stderr)

    yaml_dict = parse_yaml(sys.argv[1])
    generate_opcode(yaml_dict)
    generate_name_opcode_switch_case(yaml_dict)
    generate_name_to_opcode()

    return

if __name__ == '__main__':
    main()
