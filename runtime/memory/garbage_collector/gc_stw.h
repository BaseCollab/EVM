#ifndef EVM_RUNTIME_GARBAGE_COLLECTOR_STW_H
#define EVM_RUNTIME_GARBAGE_COLLECTOR_STW_H

#include "runtime/memory/garbage_collector/gc_base.h"
#include "runtime/memory/object_header.h"

#include <fstream>

namespace evm::runtime {

class GarbageCollectorSTW : public GarbageCollector {
public:
    // Each N_INSTRS_FREQUENCY instructions CleanMemory() is invoked
    static constexpr size_t N_INSTRS_FREQUENCY_DEFAULT = 1;

public:
    NO_COPY_SEMANTIC(GarbageCollectorSTW);
    NO_MOVE_SEMANTIC(GarbageCollectorSTW);

    GarbageCollectorSTW() : GarbageCollector() {}
    ~GarbageCollectorSTW() {}

    void UpdateState();

    bool SetInstrsFrequency(size_t n_instr_frequency);
    size_t GetInstrsFrequency() const;

    void CleanMemory();

private:
    void Mark();
    void Sweep();

    void MarkObjectRecursive(ObjectHeader *obj);

private:
    size_t n_instr_frequency_ {N_INSTRS_FREQUENCY_DEFAULT};
    size_t instrs_counter_ {0};

    size_t n_completed_marks_ {0};
    size_t n_completed_sweeps_ {0};

#ifdef GC_STW_DEBUG_ON
    std::ofstream dump_file_;
#endif // GC_STW_DEBUG_ON
};

} // namespace evm::runtime

#endif // EVM_RUNTIME_GARBAGE_COLLECTOR_STW_H
