#include "asm2byte.h"

namespace evm::asm2byte {

int Main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    auto asm2byte = AsmToByte();
    asm2byte.ParseAsmFile("/home/stanislav/EVM/assembler/asm2byte/example.easm");
    return 0;
}

} // namespace evm::asm2byte

int main(int argc, char *argv[])
{
    return evm::asm2byte::Main(argc, argv);
}
