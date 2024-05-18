#include "runtime/memory/garbage_collector/gc_stw.h"
#include "runtime/memory/allocator/allocator.h"
#include "runtime/memory/frame.h"

#include <vector>
#include <bitset>
#include <iostream>

namespace evm::runtime {

void GarbageCollectorSTW::Mark(Interpreter *interpreter)
{
    const std::vector<Frame> frames = interpreter->GetFramesStack();
    for (size_t i = 0, size = frames.size(); i < size; ++i) {
        auto frame_reg_bitset = frames[i].GetObjectBitMask();
        for (size_t reg = 0; reg < frame_reg_bitset.size(); ++reg) {
            if (frame_reg_bitset.test(reg) == true) {
                std::cout << "frame " << i << " reg " <<  reg << "\n";
            }
        }
    }
}

void GarbageCollectorSTW::Sweep(AllocatorBase *allocator)
{
    (void) allocator;
    // TODO: delete all marked object via allocator
}

} // namespace evm::runtime
