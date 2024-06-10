#include <iostream>

#include "assembler/asm2byte/asm2byte.h"
#include "runtime/runtime.h"

namespace evm {

int Main(int argc, char *argv[])
{
    // TODO: improve error handling
    if (argc != 2) {
        std::cerr << "input file required" << std::endl;
        return 1;
    }

    asm2byte::AsmToByte asm2byte;
    file_format::File file;
    asm2byte.ParseAsmFile(argv[1], &file);

    if (!runtime::Runtime::Create()) {
        std::cerr << "Failed to create runtime" << std::endl;
        return 1;
    }

    runtime::Runtime::GetInstance()->Execute(&file);

    return 0;
}

} // namespace evm

int main(int argc, char *argv[])
{
    return evm::Main(argc, argv);
}
