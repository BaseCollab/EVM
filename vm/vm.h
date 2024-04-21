#ifndef EVM_VM_VM_H
#define EVM_VM_VM_H

#include "common/constants.h"
#include "common/config.h"
#include "common/macros.h"
#include "runtime/interpreter/interpreter.h"

#include "runtime/memory/allocator/allocator.h"

#include <memory>

namespace evm {

class VirtualMachine {
public:
    NO_COPY_SEMANTIC(VirtualMachine);
    NO_MOVE_SEMANTIC(VirtualMachine);

    explicit VirtualMachine();
    ~VirtualMachine() = default;

    void Execute(const byte_t *bytecode)
    {
        interpreter_->Run(bytecode);
    }

    Interpreter *GetInterpreter()
    {
        return interpreter_.get();
    }

    memory::AllocatorBase *GetAllocator() const
    {
        return allocator_.get();
    }

private:
    std::unique_ptr<Interpreter> interpreter_;
    std::unique_ptr<memory::AllocatorBase> allocator_;
};

} // namespace evm

#endif // EVM_VM_VM_H
