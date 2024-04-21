#include "vm.h"

#include <cstring>
#include <iostream>

namespace evm {

int Main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    // TODO: add start execution from bytecode file
    return 0;
}

} // namespace evm

int main(int argc, char *argv[])
{
    return evm::Main(argc, argv);
}
