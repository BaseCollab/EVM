#include "vm.h"
#include "examples/solve_square.h"

#include <cstring>
#include <iostream>

namespace evm {

int Main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    VirtualMachine vm;

    std::cout << "[DISCRIMINANT EXAMPLE]" << std::endl;

    // clang-format off

    byte_t bytecode_discr[] = {
        Opcode::SCANF, 0x1, 0, 0,
        Opcode::SCANF, 0x2, 0, 0,
        Opcode::SCANF, 0x3, 0, 0,

        Opcode::MULF, 0x4, 0x2, 0x2,
        Opcode::MULF, 0x5, 0x1, 0x3,
        Opcode::MOVIF, 0x6, 0, 0, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        Opcode::CONVIF, 0x7, 0x6, 0x0,
        Opcode::MULF, 0x8, 0x5, 0x7,
        Opcode::SUBF, 0x9, 0x4, 0x8,

        Opcode::PRINTF, 0, 0x9, 0,

        Opcode::EXIT, 0, 0, 0
    };

    vm.Execute(bytecode_discr);

    // std::cout << "[SIN COS EXAMPLE]" << std::endl;

    // byte_t bytecode_sin_cos[] =
    // {
    //     Opcode::SCANF, FReg::XF6, 0, 0, // scan(a)
    //     Opcode::SCANF, FReg::XF7, 0, 0, // scan(b)
    //     Opcode::SCANF, FReg::XF8, 0, 0, // scan(x)

    //     Opcode::SIN, FReg::XF9, XF8, 0,
    //     Opcode::COS, FReg::XFA, XF8, 0,

    //     Opcode::MULF, FReg::XFB, FReg::XF6, FReg::XF9, // XFB = a * sin(x)
    //     Opcode::MULF, FReg::XFC, FReg::XF7, FReg::XFA, // XFC = b * cos(x)

    //     Opcode::ADDF, FReg::XFD, FReg::XFB, FReg::XFC, // XFC = XFB + XFC

    //     Opcode::PRINTF, 0, FReg::XFD, 0, // print(XFC)

    //     Opcode::EXIT, 0, 0, 0
    // };

    // vm.Execute(bytecode_sin_cos);

    // clang-format on

    return 0;
}

} // namespace evm

int main(int argc, char *argv[])
{
    return evm::Main(argc, argv);
}
