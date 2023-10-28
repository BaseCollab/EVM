#include "vm.h"

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

    std::cout << "[SIN COS EXAMPLE]" << std::endl;

    byte_t bytecode_sin_cos[] =
    {
        Opcode::SCANF, 0x1, 0, 0,
        Opcode::SCANF, 0x2, 0, 0,
        Opcode::SCANF, 0x3, 0, 0,

        Opcode::SIN, 0x4, 0x3, 0,
        Opcode::COS, 0x5, 0x3, 0,

        Opcode::MULF, 0x6, 0x1, 0x4,
        Opcode::MULF, 0x7, 0x2, 0x5,

        Opcode::ADDF, 0x8, 0x6, 0x7,
        Opcode::PRINTF, 0, 0x8, 0,

        Opcode::EXIT, 0, 0, 0
    };

    vm.Execute(bytecode_sin_cos);

    // clang-format on

    return 0;
}

} // namespace evm

int main(int argc, char *argv[])
{
    return evm::Main(argc, argv);
}
