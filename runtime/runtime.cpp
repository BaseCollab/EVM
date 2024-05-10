#include "runtime/runtime.h"

#include <iostream>

namespace evm::runtime {

Runtime *Runtime::instance_ = nullptr;

/* static */
bool Runtime::Create()
{
    if (instance_ != nullptr) {
        std::cerr << "Runtime already exists." << std::endl;
        return false;
    }

    instance_ = new Runtime();
    if (instance_ == nullptr) {
        std::cerr << "Error when allocating runtime." << std::endl;
        return false;
    }

    instance_->InitializeRuntime();

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

void Runtime::InitializeRuntime()
{
    heap_manager_ = std::make_unique<HeapManager>(DEFAULT_HEAP_SIZE);
    interpreter_ = std::make_unique<Interpreter>();
}

} // namespace evm::runtime
