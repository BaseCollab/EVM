#include "common/logs.h"
#include "assembler/asm2byte/asm2byte.h"
#include "runtime/runtime.h"

namespace evm {

int Main(int argc, char *argv[])
{
    if (argc != 2) {
        PrintErr("Input file required");
        return 1;
    }

    asm2byte::AsmToByte asm2byte;
    file_format::File file;
    asm2byte.ParseAsmFile(argv[1], &file);

    if (!runtime::Runtime::Create()) {
        PrintErr("Failed to create runtime");
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
