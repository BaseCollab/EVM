#ifndef EVM_ISA_OPCODES_ISA_OPCODES_H
#define EVM_ISA_OPCODES_ISA_OPCODES_H

#include "common/constants.h"

namespace evm {
// clang-format off
enum Opcode {
    EXIT = 0x0,

    // Arithmetic operations
    ADD  = 0x1,
    SUB  = 0x2,
    MUL  = 0x3,
    DIV  = 0x4,
    AND  = 0x5,
    OR   = 0x6,
    XOR  = 0x7,

    // Movs operations
    MOVI = 0x8,
    MOVR = 0x9,

    // Comparison operations
    SLTS = 0xa,
    SLTU = 0xb,
    SMES = 0xc,
    SMEU = 0xd,
    EQ   = 0xe,

    // Intrinsics
    PRINT = 0xf,
    SCAN  = 0x10,

    INVALID = 0x11,
};
// clang-format on

static constexpr size_t N_OPCODES = Opcode::INVALID;
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
