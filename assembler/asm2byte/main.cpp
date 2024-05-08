#include "asm2byte.h"
#include <iostream>

namespace evm::asm2byte {

int Main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Only filename arg is required" << std::endl;
        return EXIT_FAILURE;
    }

    auto asm2byte = AsmToByte();
    bool parsed = asm2byte.ParseAsmFile(argv[1]);
    if (!parsed) {
        std::cerr << "Error when parsing asm file '" << argv[1] << "'" << std::endl;
    }

    bool emitted = asm2byte.EmitBytecode();
    if (!emitted) {
        std::cerr << "Error when emitting bytecode of file '" << argv[1] << "'" << std::endl;
    }

    bool dumped = asm2byte.DumpBytesInBytecode("out.ea");
    if (!dumped) {
        std::cerr << "Error when dumping bytecode of file '" << argv[1] << "'" << std::endl;
    }

    return EXIT_SUCCESS;
}

} // namespace evm::asm2byte

int main(int argc, char *argv[])
{
    return evm::asm2byte::Main(argc, argv);
}
