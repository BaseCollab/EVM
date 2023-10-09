#ifndef EVM_ISA_REGS_ISA_REGS_H
#define EVM_ISA_REGS_ISA_REGS_H

#include "common/config.h"

namespace evm {

using reg_idx_t  = byte_t;
using freg_idx_t = reg_idx_t;
static constexpr size_t N_REG  = (1 << 5);
static constexpr size_t N_FREG = (1 << 4);

// clang-format off
enum Reg {
    X0  = 0x0,
    X1  = 0x1,
    X2  = 0x2,
    X3  = 0x3,
    X4  = 0x4,
    X5  = 0x5,
    X6  = 0x6,
    X7  = 0x7,
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
    X17 = 0x17,
    X18 = 0x18,
    X19 = 0x19,
    X1A = 0x1a,
    X1B = 0x1b,
    X1C = 0x1c,
    X1D = 0x1d,
    X1E = 0x1e,
    X1F = 0x1f,
};

enum FReg {
    XF0  = 0x0,
    XF1  = 0x1,
    XF2  = 0x2,
    XF3  = 0x3,
    XF4  = 0x4,
    XF5  = 0x5,
    XF6  = 0x6,
    XF7  = 0x7,
    XF8  = 0x8,
    XF9  = 0x9,
    XFA  = 0xa,
    XFB  = 0xb,
    XFC  = 0xc,
    XFD  = 0xd,
    XFE  = 0xe,
    XFF  = 0xf,
};
// clang-format on

} // namespace evm

#endif // EVM_ISA_REGS_ISA_REGS_H
