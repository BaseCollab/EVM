#ifndef EVM_SOLVE_SQUARE_SOLVE_SQUARE_H
#define EVM_SOLVE_SQUARE_SOLVE_SQUARE_H

#include "common/config.h"
#include "isa/opcodes.h"
#include "isa/regs.h"

namespace evm::examples {

// clang-format off
static constexpr byte_t EXAMPLE_BYTES_SOLVE_SQUARE[] = {
    PUT_IND_INSN(SCANF, XF6), // scan(a)
    PUT_IND_INSN(SCANF, XF7), // scan(b)
    PUT_IND_INSN(SCANF, XF8), // scan(c)

    PUT_A_INSN(MULF, XFB, XF7, XF7), // XFB = b^2
    PUT_A_INSN(MULF, XFC, XF6, XF8), // XFC = a * c
    PUT_IND_INSN(MOVF, XFE), 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // XFE = 0
    PUT_A_INSN(MULF, XFC, XFC, XFE), // XFC = XFC * 4
    PUT_A_INSN(SUBF, XFA, XFB, XFC), // XFA = b^2 - 4ac

    PUT_INS_INSN(PRINTF, XFA), // print(XFA)

    PUT_INSN(EXIT), // exit
};
// clang-format on

} // namespace evm::examples

#endif // EVM_SOLVE_SQUARE_SOLVE_SQUARE_H
