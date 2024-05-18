#ifndef EVM_RUNTIME_GARBAGE_COLLECTOR_STW_H
#define EVM_RUNTIME_GARBAGE_COLLECTOR_STW_H

#include "runtime/memory/garbage_collector/gc_base.h"

namespace evm::runtime {

class GarbageCollectorSTW : public GarbageCollector {
public:
    NO_COPY_SEMANTIC(GarbageCollectorSTW);
    NO_MOVE_SEMANTIC(GarbageCollectorSTW);

    GarbageCollectorSTW() :
        GarbageCollector()
    {}
    ~GarbageCollectorSTW() = default;

    void Mark(Interpreter *interpreter);
    void Sweep(AllocatorBase *allocator);
};

} // namespace evm::runtime

#endif // EVM_RUNTIME_GARBAGE_COLLECTOR_STW_H
