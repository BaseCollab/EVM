#ifndef EVM_RUNTIME_GARBAGE_COLLECTOR_INCREMENTAL_H
#define EVM_RUNTIME_GARBAGE_COLLECTOR_INCREMENTAL_H

#include "runtime/memory/garbage_collector/gc_base.h"
#include "runtime/memory/object_header.h"

#include <fstream>
#include <queue>

namespace evm::runtime {

class GarbageCollectorIncremental : public GarbageCollector {
public:
    // Each N_INSTRS_FREQUENCY instructions CleanMemory() is invoked
    static constexpr size_t N_MARK_INSTRS_FREQUENCY_DEFAULT = 1;
    static constexpr size_t N_MARKS_SWEEP_PERIOD_RATIO = 10;

public:
    NO_COPY_SEMANTIC(GarbageCollectorIncremental);
    NO_MOVE_SEMANTIC(GarbageCollectorIncremental);

    GarbageCollectorIncremental() : GarbageCollector() {}
    ~GarbageCollectorIncremental() {}

    void UpdateState();
    void CleanMemory();

    bool SetInstrsFrequency(size_t n_instr_frequency);
    size_t GetInstrsFrequency() const;

private:
    void MarkStep();
    void MarkFinalize();
    void Sweep();

private:
    size_t n_instr_frequency_ {N_MARK_INSTRS_FREQUENCY_DEFAULT};
    size_t instrs_counter_ {0}; // control marking phase frequency

    size_t n_completed_marks_ {0}; // cleaned up after each sweep
    size_t n_completed_sweeps_ {0};

    std::queue<ObjectHeader *> grey_objects_;

#ifdef GC_INCREMENTAL_DEBUG_ON
    std::ofstream dump_file_;
#endif // GC_INCREMENTAL_DEBUG_ON
};

} // namespace evm::runtime

#endif // EVM_RUNTIME_GARBAGE_COLLECTOR_INCREMENTAL_H
