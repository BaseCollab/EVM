#ifndef EVM_ISA_OPCODES_ISA_OPCODES_H
#define EVM_ISA_OPCODES_ISA_OPCODES_H

#include "common/constants.h"

namespace evm {
// clang-format off
enum Opcode {
    EXIT  = 0x0,

    // Arithmetic signed operations
    ADD   = 0x1,
    SUB   = 0x2,
    MUL   = 0x3,
    DIV   = 0x4,
    AND   = 0x5,
    OR    = 0x6,
    XOR   = 0x7,

    // Arithmetic unsigned operations
    MULU  = 0x8,
    DIVU  = 0x9,

    // Movs operations
    MOVR  = 0xa,
    MOVI  = 0xb,
    MOVF  = 0xc,
    MOVFR = 0xd,

    // Comparison integer operations
    SLT   = 0xe,
    SLTU  = 0xf,
    SME   = 0x10,
    SMEU  = 0x11,
    EQ    = 0x12,
    NEQ   = 0x13,

    // Floating point operations
    ADDF  = 0x14,
    SUBF  = 0x15,
    MULF  = 0x16,
    DIVF  = 0x17,

    // Comparison floating point operations
    SLTF  = 0x18,
    SMEF  = 0x19,
    EQF   = 0x1a,
    NEQF  = 0x1b,

    // Register conversions
    CONVRSF = 0x1c,
    CONVFRS = 0x1d,
    CONVRUF = 0x1e,
    CONVFRU = 0x1f,

    // Intrinsics
    PRINT  = 0x20,
    PRINTU = 0x21,
    PRINTF = 0x22,
    SCAN   = 0x23,
    SCANU  = 0x24,
    SCANF  = 0x25,

    INVALID = 0x26,
};
// clang-format on

using insn_size_t = word_t;

// ISA Example:

// Instruction = |_______||_______|_______|_______|
//                  |        |       |       |
//                opcode    rd     rs1     rs2

#define PUT_A_INSN(opcode, rd, rs1, rs2) opcode, rd, rs1, rs2

#define PUT_R_INSN(opcode, rd, rs1) opcode, rd, rs1, 0

#define PUT_IND_INSN(opcode, rd) opcode, rd, 0, 0

#define PUT_INS_INSN(opcode, rs1) opcode, 0, rs1, 0

#define PUT_INSN(opcode) opcode, 0, 0, 0

} // namespace evm

#endif // EVM_ISA_OPCODES_ISA_OPCODES_H
