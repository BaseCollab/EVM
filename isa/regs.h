#ifndef EVM_ISA_REGS_ISA_REGS_H
#define EVM_ISA_REGS_ISA_REGS_H

#include "common/config.h"

namespace evm {

using reg_idx_t = byte_t;
static constexpr size_t N_REG = (1 << 5);

// clang-format off
enum Reg {
    X0  = 0x0,
    X1  = 0x1,
    X2  = 0x2,
    X3  = 0x3,
    X4  = 0x4,
    X5  = 0x5,
    X6  = 0x6,
    X7  = 0x8,
    X8  = 0x8,
    X9  = 0x9,
    XA  = 0xa,
    XB  = 0xb,
    XC  = 0xc,
    XD  = 0xd,
    XE  = 0xe,
    XF  = 0xf,
    X10 = 0x10,
    X11 = 0x11,
    X12 = 0x12,
    X13 = 0x13,
    X14 = 0x14,
    X15 = 0x15,
    X16 = 0x16,
    X17 = 0x18,
    X18 = 0x18,
    X19 = 0x19,
    X1A = 0x1a,
    X1B = 0x1b,
    X1C = 0x1c,
    X1D = 0x1d,
    X1E = 0x1e,
    X1F = 0x1f,
};
// clang-format on

} // namespace evm

#endif // EVM_ISA_REGS_ISA_REGS_H
