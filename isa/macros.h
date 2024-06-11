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

#define ISA_GET_ARRAY_TYPE(instr_ptr) *(reinterpret_cast<const hword_t *>(instr_ptr + 2))
#define ISA_GET_ARRAY_SIZE_RS(instr_ptr) *(instr_ptr + 4)

/// All types are always objects, not need additional checks
#define ISA_GET_OBJ_TYPE(instr_ptr) *(reinterpret_cast<const hword_t *>(instr_ptr + 2))
#define ISA_GET_OBJ_RS(instr_ptr) *(instr_ptr + 4)
#define ISA_GET_OBJ_FIELD_TYPE(instr_ptr) *(instr_ptr + 5)
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

// clang-format on

} // namespace evm

#endif // EVM_ISA_MACROS_H
