#include "asm2byte.h"
#include <cstdlib>
#include <iostream>

namespace evm::asm2byte {

int Main(int argc, char *argv[])
{
    if (argc != 2) {
        PrintErr("Only filename arg is required");
        return EXIT_FAILURE;
    }

    file_format::File file_arch("out.ea");

    auto asm2byte = AsmToByte();
    bool parsed = asm2byte.ParseAsmFile(argv[1], &file_arch);
    if (!parsed) {
        PrintErr("Error when parsing asm file '", argv[1], "'");
        return EXIT_FAILURE;
    }

    bool dumped = file_arch.EmitBytecode();
    if (!dumped) {
        PrintErr("Error when dumping bytecode of file '", argv[1], "'");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

} // namespace evm::asm2byte

int main(int argc, char *argv[])
{
    return evm::asm2byte::Main(argc, argv);
}
