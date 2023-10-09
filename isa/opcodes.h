#ifndef EVM_ISA_OPCODES_ISA_OPCODES_H
#define EVM_ISA_OPCODES_ISA_OPCODES_H

#include "common/constants.h"

namespace evm {
// clang-format off
enum Opcode {
    EXIT = 0x0,

    // Arithmetic unsigned operations
    ADD  = 0x1,
    SUB  = 0x2,
    MUL  = 0x3,
    DIV  = 0x4,
    AND  = 0x5,
    OR   = 0x6,
    XOR  = 0x7,

    // Arithmetic signed operations
    MULU = 0x8,
    DIVU = 0x9,

    // Movs operations
    MOVI = 0xa,
    MOVR = 0xb,

    // Comparison operations
    SLT  = 0xc,
    SLTU = 0xd,
    SME  = 0xe,
    SMEU = 0xf,
    EQ   = 0x10,
    NEQ  = 0x11,

    // Intrinsics
    PRINT  = 0x12,
    PRINTU = 0x13,
    SCAN   = 0x14,

    INVALID = 0x15,
};
// clang-format on

using insn_size_t = word_t;

// ISA Example:

// Instruction = |_______||_______|_______|_______|
//                  |        |       |       |
//                opcode    rd     rs1     rs1

#define PUT_A_INSN(opcode, rd, rs1, rs2) opcode, rd, rs1, rs2

#define PUT_R_INSN(opcode, rd, rs1) opcode, rd, rs1, 0

#define PUT_IND_INSN(opcode, rd) opcode, rd, 0, 0

#define PUT_INS_INSN(opcode, rs1) opcode, 0, rs1, 0

#define PUT_INSN(opcode) opcode, 0, 0, 0

} // namespace evm

#endif // EVM_ISA_OPCODES_ISA_OPCODES_H
