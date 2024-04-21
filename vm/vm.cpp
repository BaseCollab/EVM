#include "vm.h"
#include "runtime/memory/allocator/bump_allocator.h"

namespace evm {

VirtualMachine::VirtualMachine()
{
    interpreter_ = std::make_unique<Interpreter>(this);
    allocator_ = std::make_unique<memory::BumpAllocator>(/* default size*/);
}

} // namespace evm
