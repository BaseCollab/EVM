#include "runtime/memory/garbage_collector/gc_incremental.h"
#include "runtime/memory/frame.h"
#include "runtime/memory/types/array.h"
#include "runtime/runtime.h"
#include "runtime/memory/type.h"
#include "runtime/memory/types/class.h"

#include <vector>
#include <bitset>
#include <iostream>
// #include <fstream>
// #include <string>

namespace evm::runtime {

void GarbageCollectorIncremental::MarkRoots()
{
    auto interpreter = runtime::Runtime::GetInstance()->GetInterpreter();

    const std::vector<Frame> &frames = interpreter->GetFramesStack();
    for (size_t i = 0, size = frames.size(); i < size; ++i) {
        auto &frame_reg_bitset = frames[i].GetObjectBitMask();
        for (size_t reg = 0; reg < frame_reg_bitset.size(); ++reg) {
            if (frame_reg_bitset.test(reg) == true) {
                ObjectHeader *obj_ptr = reinterpret_cast<ObjectHeader *>(frames[i].GetReg(reg)->GetRaw());
                obj_ptr->SetMarkWord({.mark = 1}); // mark object as grey
                grey_objects_.push(reinterpret_cast<ObjectHeader *>(obj_ptr));
            }
        }
    }

    if (interpreter->IsAccumMarked()) {
        ObjectHeader *obj_ptr = reinterpret_cast<ObjectHeader *>(interpreter->GetAccum().GetRaw());
        obj_ptr->SetMarkWord({.mark = 1}); // mark object as grey
        grey_objects_.push(reinterpret_cast<ObjectHeader *>(obj_ptr));
    }
}

void GarbageCollectorIncremental::MarkStep()
{
    if (n_completed_marks_ == 0) { // no grey_objects in queue -> store roots to queue
        MarkRoots();
    } else {
        for (size_t i = 0; i < N_HANDLING_GREY_OBJECTS; ++i) {
            if (grey_objects_.empty()) {
                CleanMemory();
                return;
            }

            ObjectHeader *grey_obj = grey_objects_.front();
            MarkWord mark_word = grey_obj->GetMarkWord();

            if (mark_word.neighbour == 0) { // only if object is grey
                VisitNeighbours(grey_obj);
                grey_obj->SetMarkWord({.mark = 1, .neighbour = 1}); // mark as black object
            }

            grey_objects_.pop();
        }
    }

    n_completed_marks_++;
}

void GarbageCollectorIncremental::VisitNeighbours(ObjectHeader *obj)
{
    auto class_word = obj->GetClassWord();
    auto obj_type = class_word->GetObjectType();

    switch (obj_type) {
        case memory::Type::STRING_OBJECT: {
            obj->SetMarkWord({.mark = 1, .neighbour = 1}); // always mark black
            return;
        }
        case memory::Type::CLASS_OBJECT: {
            types::Class *cls = reinterpret_cast<types::Class *>(obj);

            for (size_t i = 0, size = class_word->GetFieldsNum(); i < size; ++i) {
                const Field &field = class_word->GetField(i);
                if (!field.IsPrimitive()) {
                    ObjectHeader *obj_ptr = reinterpret_cast<ObjectHeader *>(cls->GetField(i));
                    if (obj_ptr == nullptr) {
                        continue;
                    }

                    MarkWord mark_word = obj_ptr->GetMarkWord();
                    if (mark_word.mark == 0) {             // white object
                        obj_ptr->SetMarkWord({.mark = 1}); // make object grey
                        grey_objects_.push(obj_ptr);
                    }
                }
            }
            return;
        }
        case memory::Type::ARRAY_OBJECT: {
            types::Array *array = reinterpret_cast<types::Array *>(obj);
            memory::Type array_type = class_word->GetArrayElementType();
            size_t length = array->GetLength();

            if (memory::IsReferenceType(array_type)) {
                for (size_t i = 0; i < length; ++i) {
                    int64_t obj_ptr_val = 0;
                    array->Get(&obj_ptr_val, i);
                    if (obj_ptr_val == 0) {
                        continue;
                    }

                    ObjectHeader *obj_ptr = reinterpret_cast<ObjectHeader *>(obj_ptr_val);
                    MarkWord mark_word = obj_ptr->GetMarkWord();
                    if (mark_word.mark == 0) {             // white object
                        obj_ptr->SetMarkWord({.mark = 1}); // make object grey
                        grey_objects_.push(obj_ptr);
                    }
                }
            }
            return;
        }
        default: {
            std::cerr << "Invalid type of object in GC-incremental mark-step phase: something went wrong" << std::endl;
            return;
        }
    }
}

void GarbageCollectorIncremental::MarkFinalize()
{
    while (!grey_objects_.empty()) {
        ObjectHeader *grey_obj = grey_objects_.front();
        MarkWord mark_word = grey_obj->GetMarkWord();

        if (mark_word.neighbour == 0) { // only if object is grey
            VisitNeighbours(grey_obj);
            grey_obj->SetMarkWord({.mark = 1, .neighbour = 1}); // mark as black object
        }

        grey_objects_.pop();
    }
}

void GarbageCollectorIncremental::Sweep()
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
    if (!grey_objects_.empty()) { // need to sweep, but grey objects still exist
        MarkFinalize();
    }

    Sweep();

    n_completed_marks_ = 0;
    instrs_counter_ = 0;
}

} // namespace evm::runtime
