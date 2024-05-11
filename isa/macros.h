#ifndef EVM_ISA_MACROS_H
#define EVM_ISA_MACROS_H

#include "common/macros.h"
#include "common/constants.h"

namespace evm {

// clang-format off

using instr_size_t = word_t;

#define ISA_GET_RD(instr_ptr)  *(instr_ptr + 1)
#define ISA_GET_RS1(instr_ptr) *(instr_ptr + 2)
#define ISA_GET_RS2(instr_ptr) *(instr_ptr + 3)
#define ISA_GET_RS3(instr_ptr) ISA_GET_RD(instr_ptr)

/// TODO: Get class type if number is >= 0
#define ISA_GET_ARRAY_TYPE(instr_ptr) *(reinterpret_cast<const hword_t *>(instr_ptr + 2))

/// All types are always objects, not need additional checks
#define ISA_GET_OBJ_TYPE(instr_ptr) *(reinterpret_cast<const hword_t *>(instr_ptr + 3))
#define ISA_GET_OBJ_RS(instr_ptr) *(instr_ptr + 4)
#define ISA_GET_OBJ_FIELD_SIZE(instr_ptr) *(instr_ptr + 5)
#define ISA_GET_OBJ_OP_RS(instr_ptr) ISA_GET_RD(instr_ptr)

#define ISA_CALL_GET_REG1(instr_ptr) *(instr_ptr + 4)
#define ISA_CALL_GET_REG2(instr_ptr) *(instr_ptr + 5)
#define ISA_CALL_GET_REG3(instr_ptr) *(instr_ptr + 6)
#define ISA_CALL_GET_REG4(instr_ptr) *(instr_ptr + 7)

#define ISA_NEXT_INSTR(pc) (pc + sizeof(instr_size_t))
#define ISA_INSTR_SIZE sizeof(instr_size_t)

#define ISA_GET_IMM(instr_ptr, type)                                          \
    ({                                                                        \
        type __val = 0;                                                       \
        std::memcpy(&__val, instr_ptr + sizeof(instr_size_t), sizeof(type));  \
        __val;                                                                \
    })

// Stupid temporary DSL for testing
// TODO: write assembler and remove all this stuff
#define PUT_A_INSTR(opcode, rd, rs1, rs2)                   opcode, rd, rs1, rs2
#define PUT_R_INSTR(opcode, rd, rs1)                        opcode, rd, rs1, 0x0
#define PUT_IMM_INSTR(opcode, rd)                           opcode, rd, 0x0, 0x0
#define PUT_IMM_INSTR_BYTE(opcode, rd, byte)                opcode, rd, 0x0, 0x0, byte, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
#define PUT_INS_INSTR(opcode, rs1)                          opcode, 0x0, rs1, 0x0
#define PUT_INSTR(opcode)                                   opcode, 0x0, 0x0, 0x0
#define PUT_CALL_INSTR(opcode, rs1, reg1, reg2, reg3, reg4) opcode, 0x0, rs1, 0x0, reg1, reg2, reg3, reg4

// clang-format on

} // namespace evm

#endif // EVM_ISA_MACROS_H
