#ifndef EVM_ISA_MACROS_H
#define EVM_ISA_MACROS_H

#include "common/macros.h"
#include "common/constants.h"

namespace evm {

using instr_size_t = word_t;

#define ISA_GET_RD(instr_ptr)  *(instr_ptr + 1)
#define ISA_GET_RS1(instr_ptr) *(instr_ptr + 2)
#define ISA_GET_RS2(instr_ptr) *(instr_ptr + 3)

#define ISA_NEXT_INSTR(pc) (pc + sizeof(instr_size_t)) 

#define ISA_GET_IMM(instr_ptr, type, pc)                                         \
    ({                                                                           \
        type __val = 0;                                                          \
        std::memcpy(&__val, instr_ptr + sizeof(instr_size_t), sizeof(type));     \
        __val;                                                                   \
    })

} // namespace evm

#endif // EVM_ISA_MACROS_H
