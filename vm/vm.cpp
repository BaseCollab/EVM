#include "vm.h"
#include "runtime/runtime.h"

namespace evm {

/* static */
runtime::Runtime *EVM::CreateRuntime()
{
    if(!runtime::Runtime::Create()) {
        std::cerr << "The Runtime has already been created." << std::endl;
        return nullptr;
    }

    return runtime::Runtime::GetInstance();
}

/* static */
bool EVM::DestroyRuntime()
{
    if (!runtime::runtime::Runtime::Destroy()) {
        std::cerr << "Runtime has alreade been destroyed." << std::endl;
        return false;
    }

    return true;
}

} // namespace evm
