import yaml
import sys

def parse_yaml(filename):
    with open(filename, 'r') as instr_file:
        return yaml.safe_load(instr_file)

def generate_name_opcode_switch_case(yaml_dict):
    with open('opcode_name_map.h', 'w') as fout:
        fout.write("#ifndef EVM_ISA_OPCODE_NAME_MAP_H\n"
                   "#define EVM_ISA_OPCODE_NAME_MAP_H\n\n"
                   "#include <string_view>\n"
                   "#include <unordered_map>\n\n"
                   "#include \"opcodes.h\"\n\n"
                   "namespace evm::isa {\n\n")

        fout.write('static std::unordered_map<std::string_view, Opcode> NAME_OPCODE_MAP = {\n')

        for key in yaml_dict:
            # opcode = (yaml_dict[key])['opcode']
            fout.write(f"    {{\"{key}\", Opcode::{key.upper()}}},\n")

        fout.write("};\n\n")
        fout.write("} // namespace evm::isa\n\n")

        fout.write("#endif // EVM_ISA_OPCODE_NAME_MAP_H\n")
    return

def main():
    if len(sys.argv) != 2:
        print('Invalid amount of arguments [1 must be used]',        file=sys.stderr)
        print('Usage example: ' + sys.argv[0] + ' ' + 'isa.yaml', file=sys.stderr)

    yaml_dict = parse_yaml(sys.argv[1])
    generate_name_opcode_switch_case(yaml_dict)

    return

if __name__ == '__main__':
    main()
