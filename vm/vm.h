#ifndef EVM_VM_VM_H
#define EVM_VM_VM_H

#include "common/macros.h"

#include <memory>

namespace evm {

namespace runtime {
class Runtime;
};

class EVM {
public:
    NO_COPY_SEMANTIC(EVM);
    NO_MOVE_SEMANTIC(EVM);

    EVM() = default;
    ~EVM() = default;

    static runtime::Runtime *CreateRuntime();
    static void DestroyRuntime();
};

} // namespace evm

#endif // EVM_VM_VM_H
