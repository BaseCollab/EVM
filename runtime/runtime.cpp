#include "runtime/runtime.h"
#include "vm/vm.h"

namespace evm::runtime {

Runtime::instance_ = nullptr;

/* static */
bool Runtime::Create()
{
    if (instance_ != nullptr) {
        return false;
    }

    instance_ = new Runtime();
    
    if (instance_ == nullptr) {
        return false;
    }

    heap_manager_ = std::make_unique<HeapManager>(DEFAULT_HEAP_SIZE);
    interpreter_ = std::make_unique<Interpreter>(this);

    return true;
}

/* static */
bool Runtime::Destroy()
{
    if (instance_ == nullptr) {
        return false;
    }

    delete instance_;
    instance_ = nullptr;

    return true;
}

bool CreateVM()
{
    
}

} // namespace evm::runtime
