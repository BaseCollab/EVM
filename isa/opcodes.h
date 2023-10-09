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
    MOVR = 0xa,
    MOVI = 0xb,
    MOVF = 0xc,

    // Comparison operations
    SLT  = 0xd,
    SLTU = 0xe,
    SME  = 0xf,
    SMEU = 0x10,
    EQ   = 0x11,
    NEQ  = 0x12,

    // Floating point operations
    ADDF  = 0x13,
    SUBF  = 0x14,
    MULF  = 0x15,
    DIVF  = 0x16,

    // Register conversions
    CONVRF = 0x17,
    CONVFR = 0x18,

    // Intrinsics
    PRINT  = 0x19,
    PRINTU = 0x1a,
    PRINTF = 0x1b,
    SCAN   = 0x1c,
    SCANU  = 0x1d,
    SCANF  = 0x1e,

    INVALID = 0x1f,
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
