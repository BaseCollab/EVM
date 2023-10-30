#ifndef EVM_ISA_OPCODES_ISA_OPCODES_H
#define EVM_ISA_OPCODES_ISA_OPCODES_H

#include "common/constants.h"
#include "isa/macros.h"

namespace evm {

// clang-format off

#define DEFINE_INSTR(instr, opcode, interpret) instr = opcode,

enum Opcode : int {
    INVALID = -1,
    #include "isa/isa.def"
};

#undef DEFINE_INSTR

// clang-format on

} // namespace evm

#endif // EVM_ISA_OPCODES_ISA_OPCODES_H
