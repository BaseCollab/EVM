#include "common/opcode_to_str.h"
#include "common/utils/string_operations.h"
#include "isa/opcodes.h"

namespace evm::common {

// clang-format off

std::string OpcodeToString(Opcode opcode)
{
    #define DEFINE_INSTR(instr, opcode, interpret) \
    case opcode:                                   \
        return StrToLower(#instr);

    switch (opcode) {
        #include "isa/isa.def"

        default:
            return "invalid";
    }
    return "";

    #undef DEFINE_INSTR
}

// clang-format on

} // namespace evm::common
