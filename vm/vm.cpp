#include "vm.h"
#include "memory/allocator/bump_allocator.h"

namespace evm {

VirtualMachine::VirtualMachine()
{
    interpreter_ = std::make_unique<Interpreter>(this);
    heap_manager_ = std::make_unique<memory::HeapManager>(/* default size*/);
}

} // namespace evm
