#ifndef EVM_ISA_OPCODES_ISA_OPCODES_H
#define EVM_ISA_OPCODES_ISA_OPCODES_H

#include "common/constants.h"

namespace evm {
// clang-format off
enum Opcode {
    EXIT = 0x0,

    // Arithmetic signed operations
    ADD  = 0x1,
    SUB  = 0x2,
    MUL  = 0x3,
    DIV  = 0x4,
    AND  = 0x5,
    OR   = 0x6,
    XOR  = 0x7,

    // Arithmetic unsigned operations
    MULU = 0x8,
    DIVU = 0x9,

    // Movs operations
    MOVR = 0xa,
    MOVI = 0xb,
    MOVF = 0xc,

    // Comparison integer operations
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

    // Comparison floating point operations
    SLTF  = 0x17,
    SMEF  = 0x18,
    EQF   = 0x19,
    NEQF  = 0x1a,

    // Register conversions
    CONVRF = 0x1b,
    CONVFR = 0x1c,

    // Intrinsics
    PRINT  = 0x1d,
    PRINTU = 0x1e,
    PRINTF = 0x1f,
    SCAN   = 0x20,
    SCANU  = 0x21,
    SCANF  = 0x22,

    INVALID = 0x23,
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
