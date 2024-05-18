#include "runtime/memory/garbage_collector/gc_stw.h"
#include "runtime/memory/allocator/allocator.h"
#include "runtime/memory/frame.h"
#include "runtime/runtime.h"
#include "isa/macros.h"

#include <vector>
#include <bitset>
#include <iostream>

namespace evm::runtime {

void GarbageCollectorSTW::Mark()
{
    auto interpreter = runtime::Runtime::GetInstance()->GetInterpreter();

    const std::vector<Frame> frames = interpreter->GetFramesStack();
    for (size_t i = 0, size = frames.size(); i < size; ++i) {
        auto frame_reg_bitset = frames[i].GetObjectBitMask();
        for (size_t reg = 0; reg < frame_reg_bitset.size(); ++reg) {
            if (frame_reg_bitset.test(reg) == true) {
                // frames[i].GetReg(reg)
                std::cout << "frame " << i << " reg " << reg << "\n";
            }
        }
    }
}

void GarbageCollectorSTW::Sweep()
{
    return;
    // TODO: delete all marked object via allocator
}

void GarbageCollectorSTW::CleanMemory()
{
    Mark();
    Sweep();
}

bool GarbageCollectorSTW::SetInstrsFrequency(size_t n_instr_frequency)
{
    if (n_instr_frequency > 0) {
        return !!(n_instr_frequency_ = n_instr_frequency);
    } else {
        return false;
    }
}

size_t GarbageCollectorSTW::GetInstrsFrequency() const
{
    return n_instr_frequency_;
}

void GarbageCollectorSTW::UpdateState()
{
    instrs_counter_++;
    if (UNLIKELY(instrs_counter_ == n_instr_frequency_)) {
        instrs_counter_ = 0;
        CleanMemory();
    }
}

} // namespace evm::runtime
