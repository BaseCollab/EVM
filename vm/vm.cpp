#include "vm.h"

namespace evm {

void VirtualMachine::Execute(const byte_t *bytecode)
{
    interpreter_.Run(this, bytecode);
}

} // namespace evm
