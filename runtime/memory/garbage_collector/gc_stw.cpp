#include "runtime/memory/garbage_collector/gc_stw.h"
#include "runtime/memory/frame.h"
#include "runtime/memory/types/array.h"
#include "runtime/runtime.h"
#include "runtime/memory/type.h"
#include "runtime/memory/types/class.h"

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
    dump_file_ << "digraph G {" << std::endl << "\tcompound = true;" << std::endl;
#endif // GC_STW_DEBUG_ON

    auto interpreter = runtime::Runtime::GetInstance()->GetInterpreter();

    const std::vector<Frame> &frames = interpreter->GetFramesStack();
    for (size_t i = 0, size = frames.size(); i < size; ++i) {
        auto &frame_reg_bitset = frames[i].GetObjectBitMask();
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
            heap_manager->DeallocateObject(obj);
        }
    }

    n_completed_sweeps_++;

    return;
}

void GarbageCollectorSTW::MarkObjectRecursive(ObjectHeader *obj)
{
    auto mark_word = obj->GetMarkWord();
    if (mark_word.mark == 1) {
        return;
    }

    auto class_word = obj->GetClassWord();
    auto obj_type = class_word->GetObjectType();

    switch (obj_type) {
        case memory::Type::STRING_OBJECT: {
#ifdef GC_STW_DEBUG_ON
            dump_file_ << "\tel_" << long(obj) << " [label = \"str\"];" << std::endl;
#endif // GC_STW_DEBUG_ON
            obj->SetMarkWord({.mark = 1});
            return;
        }
        case memory::Type::CLASS_OBJECT: {
            types::Class *cls = reinterpret_cast<types::Class *>(obj);

#ifdef GC_STW_DEBUG_ON
            dump_file_ << "\tsubgraph cluster_class_" << long(obj) << " {" << std::endl
                       << "\t\tstyle = filled;\n\t\tcolor = green;" << std::endl
                       << "\t\tlabel = \"ptr: " << long(obj) << "\";" << std::endl
                       << "\t\tnode [style = filled, color = grey];" << std::endl;

            for (size_t i = 0, size = class_word->GetFieldsNum(); i < size; ++i) {
                const Field &field = class_word->GetField(i);
                dump_file_ << "\t\tel_" << long(reinterpret_cast<uint8_t *>(cls) + field.GetOffset()) << " [label = \""
                           << memory::GetStringFromType(field.GetType()) << "\"];" << std::endl;
            }

            dump_file_ << "\t}\n" << std::endl;
#endif // GC_STW_DEBUG_ON

            for (size_t i = 0, size = class_word->GetFieldsNum(); i < size; ++i) {
                const Field &field = class_word->GetField(i);
                if (!field.IsPrimitive()) {
                    reg_t obj_ptr = cls->GetField(i);
                    if (obj_ptr != 0) {
                        MarkObjectRecursive(reinterpret_cast<ObjectHeader *>(obj_ptr));
                    }

#ifdef GC_STW_DEBUG_ON
                    dump_file_ << "\tel_" << long(reinterpret_cast<uint8_t *>(cls) + field.GetOffset()) << " -> el_"
                               << long(obj_ptr) << " [lhead = cluster_" << long(obj_ptr) << "];" << std::endl;
#endif // GC_STW_DEBUG_ON
                }
            }

            cls->SetMarkWord({.mark = 1});
            return;
        }
        case memory::Type::ARRAY_OBJECT: {
            types::Array *array = reinterpret_cast<types::Array *>(obj);
            memory::Type array_type = class_word->GetArrayElementType();
            size_t length = array->GetLength();

#ifdef GC_STW_DEBUG_ON
            size_t array_type_size = GetSizeOfType(array_type);

            dump_file_ << "\tsubgraph cluster_arr_" << long(obj) << " {" << std::endl
                       << "\t\tstyle = filled;\n\t\tcolor = green;" << std::endl
                       << "\t\tlabel = \"ptr: " << long(obj) << "\";" << std::endl
                       << "\t\tnode [style = filled, color = grey];" << std::endl;

            for (size_t i = 0; i < length; ++i) {
                dump_file_ << "\t\tel_" << long(reinterpret_cast<uint8_t *>(array) + i * array_type_size)
                           << " [label = \"" << memory::GetStringFromType(array_type) << "_" << std::to_string(i)
                           << "\"];" << std::endl;
            }

            dump_file_ << "\t}\n" << std::endl;
#endif // GC_STW_DEBUG_ON

            if (memory::IsReferenceType(array_type)) {
                for (size_t i = 0; i < length; ++i) {
                    int64_t obj_ptr = 0;
                    array->Get(&obj_ptr, i);
                    if (obj_ptr != 0) {
                        MarkObjectRecursive(reinterpret_cast<ObjectHeader *>(obj_ptr));

#ifdef GC_STW_DEBUG_ON
                        dump_file_ << "\tel_" << long(reinterpret_cast<uint8_t *>(array) + i * array_type_size)
                                   << " -> el_" << long(obj_ptr) << " [lhead = cluster_" << long(obj_ptr) << "];"
                                   << std::endl;
#endif // GC_STW_DEBUG_ON
                    }
                }
            }

            array->SetMarkWord({.mark = 1});
            return;
        }
        default: {
            PrintErr("Invalid type of object in STW-GC mark phase: something went wrong");
            return;
        }
    }
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

void GarbageCollectorSTW::CleanMemory()
{
    Mark();
    Sweep();
}

} // namespace evm::runtime
