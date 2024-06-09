#include "runtime/memory/garbage_collector/gc_stw.h"
#include "runtime/memory/allocator/allocator.h"
#include "runtime/memory/frame.h"
#include "runtime/runtime.h"
#include "runtime/memory/type.h"
#include "runtime/memory/types/class.h"
#include "isa/macros.h"

#include <vector>
#include <bitset>
#include <iostream>
#include <fstream>
#include <string>

namespace evm::runtime {

void GarbageCollectorSTW::Mark()
{
#ifdef GC_STW_DEBUG_ON
    dump_file_.open(std::string("gc_stw_") + std::to_string(n_completed_marks_) + std::string(".dot"));
    dump_file_ << "digraph G {" << std::endl;
#endif // GC_STW_DEBUG_ON

    auto interpreter = runtime::Runtime::GetInstance()->GetInterpreter();

    const std::vector<Frame> frames = interpreter->GetFramesStack();
    for (size_t i = 0, size = frames.size(); i < size; ++i) {
        auto frame_reg_bitset = frames[i].GetObjectBitMask();
        for (size_t reg = 0; reg < frame_reg_bitset.size(); ++reg) {
            if (frame_reg_bitset.test(reg) == true) {
                reg_t obj_ptr = frames[i].GetReg(reg)->GetRaw();
                MarkObjectRecursive(reinterpret_cast<ObjectHeader *>(obj_ptr));
            }
        }
    }

    if (interpreter->IsAccumMarked()) {
        reg_t obj_ptr = interpreter->GetAccum().GetRaw();
        MarkObjectRecursive(reinterpret_cast<ObjectHeader *>(obj_ptr));
    }

#ifdef GC_STW_DEBUG_ON
    dump_file_ << "}" << std::endl;
    dump_file_.close();
#endif // GC_STW_DEBUG_ON

    n_completed_marks_++;
}

void GarbageCollectorSTW::Sweep()
{
    auto runtime = runtime::Runtime::GetInstance();
    auto heap_manager = runtime->GetHeapManager();
    auto objects_list = heap_manager->GetObjectsList();

    for (auto obj : objects_list) {
        auto mark_word = obj->GetMarkWord();
        if (mark_word.mark == 1) {
            obj->SetMarkWord({.mark = 0});
        } else {
            continue;
            // deallocate this obj (need new allocator)
        }
    }

    n_completed_sweeps_++;

    return;
}

void GarbageCollectorSTW::CleanMemory()
{
    Mark();
    Sweep();
}

void GarbageCollectorSTW::MarkObjectRecursive(ObjectHeader *obj)
{
    auto mark_word = obj->GetMarkWord();
    if (mark_word.mark == 1) {
        return;
    }

    auto class_word = obj->GetClassWord();

#ifdef GC_STW_DEBUG_ON
    dump_file_ << "\tN_" << long(obj) << ";" << std::endl;
#endif // GC_STW_DEBUG_ON

    // TODO: check also strings & arrays besides classes
    types::Class *cls = reinterpret_cast<types::Class *>(obj);

    for (size_t i = 0, size = class_word->GetFieldsNum(); i < size; ++i) {
        const Field field = class_word->GetField(i);
        if (!field.IsPrimitive()) {
            reg_t obj_ptr = cls->GetField(i);
            MarkObjectRecursive(reinterpret_cast<ObjectHeader *>(obj_ptr));

#ifdef GC_STW_DEBUG_ON
            dump_file_ << "\tN_" << long(obj) << " -> N_" << long(obj_ptr)  << ";" << std::endl;
#endif // GC_STW_DEBUG_ON
        }
    }

    cls->SetMarkWord({.mark = 1});
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
