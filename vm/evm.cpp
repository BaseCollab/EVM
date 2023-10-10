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
    // cllang-format off
    byte_t bytecode_discr[] =
    {
        Opcode::SCANF, FReg::XF6, 0, 0, // scan(a)
        Opcode::SCANF, FReg::XF7, 0, 0, // scan(b)
        Opcode::SCANF, FReg::XF8, 0, 0, // scan(c)

        Opcode::MULF, FReg::XFB, FReg::XF7, FReg::XF7, // XFB = b^2
        Opcode::MULF, FReg::XFC, FReg::XF6, FReg::XF8, // XFC = a * c
        Opcode::MOVFR, FReg::XFE, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // XFE = 0
        Opcode::MULF, FReg::XFC, FReg::XFC, FReg::XFE, // XFC = XFC * 4
        Opcode::SUBF, FReg::XFA, FReg::XFB, FReg::XFC, // XFA = b^2 - 4ac

        Opcode::PRINTF, 0, FReg::XFA, 0, // print(XFA)

        Opcode::EXIT, 0, 0, 0
    };

    double four = 4.0;
    std::memcpy(bytecode_discr + 6 * sizeof(insn_size_t), &four, sizeof(four));

    vm.Execute(bytecode_discr);

    std::cout << "[SIN COS EXAMPLE]" << std::endl;

    byte_t bytecode_sin_cos[] =
    {
        Opcode::SCANF, FReg::XF6, 0, 0, // scan(a)
        Opcode::SCANF, FReg::XF7, 0, 0, // scan(b)
        Opcode::SCANF, FReg::XF8, 0, 0, // scan(x)

        Opcode::SIN, FReg::XF9, XF8, 0,
        Opcode::COS, FReg::XFA, XF8, 0,
        
        Opcode::MULF, FReg::XFB, FReg::XF6, FReg::XF9, // XFB = a * sin(x)
        Opcode::MULF, FReg::XFC, FReg::XF7, FReg::XFA, // XFC = b * cos(x)

        Opcode::ADDF, FReg::XFD, FReg::XFB, FReg::XFC, // XFC = XFB + XFC

        Opcode::PRINTF, 0, FReg::XFD, 0, // print(XFC)

        Opcode::EXIT, 0, 0, 0
    };
    //clang-format on
    vm.Execute(bytecode_sin_cos);

    return 0;
}

} // namespace evm

int main(int argc, char *argv[])
{
    return evm::Main(argc, argv);
}
