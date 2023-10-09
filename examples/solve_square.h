#ifndef EVM_SOLVE_SQUARE_SOLVE_SQUARE_H
#define EVM_SOLVE_SQUARE_SOLVE_SQUARE_H

#include "common/config.h"
#include "isa/opcodes.h"
#include "isa/regs.h"

namespace evm::examples {

// clang-format off
static constexpr byte_t EXAMPLE_BYTES_SOLVE_SQUARE[] = {
    PUT_A_INSN(XOR, X6, X6, X6), // a = 0
    PUT_A_INSN(XOR, X7, X7, X7), // b = 0
    PUT_A_INSN(XOR, X8, X8, X8), // c = 0

    PUT_IND_INSN(SCAN, X6), // scan(a)
    PUT_IND_INSN(SCAN, X7), // scan(b)
    PUT_IND_INSN(SCAN, X8), // scan(c)

    PUT_A_INSN(MUL, XB, X7, X7), // XB = b^2
    PUT_A_INSN(MUL, XC, X6, X8), // XC = a * c
    PUT_IND_INSN(MOVI, XE), 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // XE = 4
    PUT_A_INSN(MUL, XC, XC, XE), // XC = XC * 4
    PUT_A_INSN(SUB, XA, XB, XC), // XC = XC * 4

    PUT_INS_INSN(PRINT, XA), // print(b^2 - 4ac)

    PUT_INSN(EXIT), // exit
};
// clang-format on

} // evm

#endif // EVM_SOLVE_SQUARE_SOLVE_SQUARE_H
