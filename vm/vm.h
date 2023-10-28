#ifndef EVM_VM_H
#define EVM_VM_H

#include "common/constants.h"
#include "common/config.h"
#include "common/macros.h"
#include "interpreter/interpreter.h"

#include <cassert>

namespace evm {

class VirtualMachine {
public:
    NO_COPY_SEMANTIC(VirtualMachine);
    NO_MOVE_SEMANTIC(VirtualMachine);

    VirtualMachine() = default;
    ~VirtualMachine() = default;

    void Execute(const byte_t *bytecode);

private:
    Interpreter interpreter_;
};

} // namespace evm

#endif // EVM_VM_H
