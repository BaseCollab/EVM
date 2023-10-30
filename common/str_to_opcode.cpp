#include "common/str_to_opcode.h"
#include "common/utils/string_operations.h"
#include "isa/opcodes.h"

#include <unordered_map>

namespace evm::common {

// clang-format off

Opcode StringToOpcode(const std::string &opcode_str)
{
    static std::unordered_map<std::string, Opcode> str_opcode_map;

    #define DEFINE_INSTR(instr, opcode, interpret) str_opcode_map[StrToLower(#instr)] = static_cast<Opcode>(opcode);

    #include "isa/isa.def"

    #undef DEFINE_INSTR

    auto it = str_opcode_map.find(opcode_str);
    if (it == str_opcode_map.end()) {
        return Opcode::INVALID;
    }

    return it->second;
}

// clang-format on

} // namespace evm::common
