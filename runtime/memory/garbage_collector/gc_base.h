#ifndef EVM_RUNTIME_GARBAGE_COLLECTOR_H
#define EVM_RUNTIME_GARBAGE_COLLECTOR_H

#include "runtime/interpreter/interpreter.h"
#include "runtime/memory/allocator/allocator.h"

namespace evm::runtime {

class GarbageCollector {
public:
    NO_COPY_SEMANTIC(GarbageCollector);
    NO_MOVE_SEMANTIC(GarbageCollector);

    GarbageCollector() = default;
    ~GarbageCollector() = default;

    // Must be called after each instruction
    virtual void UpdateState() = 0;
    virtual void CleanMemory() = 0;
};

} // namespace evm::runtime

#endif // EVM_RUNTIME_GARBAGE_COLLECTOR_H
