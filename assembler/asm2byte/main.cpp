#include "asm2byte.h"

namespace evm::asm2byte {

int Main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Only filename arg required" << std::endl;
        return EXIT_FAILURE;
    }

    auto asm2byte = AsmToByte();
    asm2byte.ParseAsmFile(argv[1]);
    asm2byte.DumpInstructionsToBytes();
    asm2byte.DumpBytesInBytecode("out.ea");

    return EXIT_SUCCESS;
}

} // namespace evm::asm2byte

int main(int argc, char *argv[])
{
    return evm::asm2byte::Main(argc, argv);
}
