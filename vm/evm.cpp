#include "vm.h"
#include "examples/solve_square.h"

namespace evm {

int Main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    VirtualMachine vm;
    vm.Execute(examples::EXAMPLE_BYTES_SOLVE_SQUARE);

    return 0;
}

} // namespace evm

int main(int argc, char *argv[])
{
    return evm::Main(argc, argv);
}
