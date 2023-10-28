#include "vm.h"

namespace evm {

void VirtualMachine::Execute(const byte_t *bytecode)
{
    interpreter_.Run(bytecode);
}

Interpreter *VirtualMachine::GetInterpreter()
{
    return &interpreter_;
}

} // namespace evm
