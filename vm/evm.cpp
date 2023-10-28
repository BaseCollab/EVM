#include "vm.h"

#include <cstring>
#include <iostream>

namespace evm {

int Main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    // VirtualMachine vm;

    std::cout << "[DISCRIMINANT EXAMPLE]" << std::endl;

    // clang-format off

    byte_t bytecode_discr[] = {
        PUT_IMM_INSTR(Opcode::SCANF, 0x1),
        PUT_IMM_INSTR(Opcode::SCANF, 0x2),
        PUT_IMM_INSTR(Opcode::SCANF, 0x3),

        PUT_A_INSTR(Opcode::MULF, 0x4, 0x2, 0x2),
        PUT_A_INSTR(Opcode::MULF, 0x5, 0x1, 0x3),

        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x6, 0x4),
        PUT_A_INSTR(Opcode::CONVIF, 0x7, 0x6, 0x0),
        PUT_A_INSTR(Opcode::MULF, 0x8, 0x5, 0x7),
        PUT_A_INSTR(Opcode::SUBF, 0x9, 0x4, 0x8),

        PUT_INS_INSTR(Opcode::PRINTF, 0x9),

        PUT_INSTR(Opcode::EXIT)
    };

    vm.Execute(bytecode_discr);

    // std::cout << "[SIN COS EXAMPLE]" << std::endl;

    byte_t bytecode_sin_cos[] =
    {
        PUT_IMM_INSTR(Opcode::SCANF, 0x1),
        PUT_IMM_INSTR(Opcode::SCANF, 0x2),
        PUT_IMM_INSTR(Opcode::SCANF, 0x3),

        PUT_R_INSTR(Opcode::SIN, 0x4, 0x3),
        PUT_R_INSTR(Opcode::COS, 0x5, 0x3),

        PUT_A_INSTR(Opcode::MULF, 0x6, 0x1, 0x4),
        PUT_A_INSTR(Opcode::MULF, 0x7, 0x2, 0x5),

        PUT_A_INSTR(Opcode::ADDF, 0x8, 0x6, 0x7),
        PUT_INS_INSTR(Opcode::PRINTF, 0x8),

        PUT_INSTR(Opcode::EXIT)
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
