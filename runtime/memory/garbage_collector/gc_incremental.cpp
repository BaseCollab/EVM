#include "runtime/memory/garbage_collector/gc_incremental.h"
#include "runtime/memory/frame.h"
// #include "runtime/memory/types/array.h"
#include "runtime/runtime.h"
// #include "runtime/memory/type.h"
// #include "runtime/memory/types/class.h"

#include <vector>
#include <bitset>
// #include <iostream>
// #include <fstream>
// #include <string>

namespace evm::runtime {

void GarbageCollectorIncremental::MarkStep()
{
    if (n_completed_marks_ == 0) // no grey_objects in queue
    {
        auto interpreter = runtime::Runtime::GetInstance()->GetInterpreter();

        // const std::vector<Frame> &frames = interpreter->GetFramesStack();
        // for (size_t i = 0, size = frames.size(); i < size; ++i) {
        //     auto &frame_reg_bitset = frames[i].GetObjectBitMask();
        //     for (size_t reg = 0; reg < frame_reg_bitset.size(); ++reg) {
        //         if (frame_reg_bitset.test(reg) == true) {
        //             reg_t obj_ptr = frames[i].GetReg(reg)->GetRaw();
        //             MarkObjectRecursive(reinterpret_cast<ObjectHeader *>(obj_ptr));
        //         }
        //     }
        // }

        // if (interpreter->IsAccumMarked()) {
        //     reg_t obj_ptr = interpreter->GetAccum().GetRaw();
        //     MarkObjectRecursive(reinterpret_cast<ObjectHeader *>(obj_ptr));
        // }

    }


    n_completed_marks_++;
}


bool GarbageCollectorIncremental::SetInstrsFrequency(size_t n_instr_frequency)
{
    if (n_instr_frequency > 0) {
        return !!(n_instr_frequency_ = n_instr_frequency);
    } else {
        return false;
    }
}

size_t GarbageCollectorIncremental::GetInstrsFrequency() const
{
    return n_instr_frequency_;
}

void GarbageCollectorIncremental::UpdateState()
{
    instrs_counter_++;
    if (UNLIKELY((instrs_counter_ % n_instr_frequency_) == 0)) {
        MarkStep();
    }

    if (UNLIKELY(instrs_counter_ == (n_instr_frequency_ * N_MARKS_SWEEP_PERIOD_RATIO))) {
        CleanMemory();
    }
}

void GarbageCollectorIncremental::CleanMemory()
{
    MarkFinalize();
    Sweep();

    n_completed_marks_ = 0;
}

} // namespace evm::runtime
