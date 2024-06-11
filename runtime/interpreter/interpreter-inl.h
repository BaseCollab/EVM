#ifndef EVM_RUNTIME_INTERPRETER_INL_H
#define EVM_RUNTIME_INTERPRETER_INL_H

#include "common/logs.h"
#include "runtime/interpreter/interpreter.h"
#include "runtime/memory/types/array.h"
#include "runtime/memory/types/string.h"
#include "runtime/runtime.h"
#include "file_format/file.h"
#include "runtime/memory/type.h"
#include "runtime/memory/types/class.h"

namespace evm::runtime {

ALWAYS_INLINE int64_t HandleCreateArrayObject(hword_t type, int32_t size)
{
    auto array_type = static_cast<memory::Type>(type);

    auto *array_obj = types::Array::Create(array_type, size);
    if (UNLIKELY(array_obj == nullptr)) {
        PrintErr("Error when creating object for array of type \"", GetStringFromType(array_type).c_str(), "\"");
        UNREACHABLE();
    }

    return reinterpret_cast<int64_t>(array_obj);
}

ALWAYS_INLINE int64_t HandleGetArraySize(int64_t array_ptr)
{
    auto *array = reinterpret_cast<types::Array *>(array_ptr);
    return array->GetLength();
}

ALWAYS_INLINE int64_t HandleLoadFromArray(int64_t array_ptr, int64_t idx, bool *load_obj)
{
    auto *array = reinterpret_cast<types::Array *>(array_ptr);
    assert(array != nullptr);

    int64_t value = 0;
    array->Get(&value, idx);

    auto arr_type = array->GetClassWord()->GetArrayElementType();
    bool is_primitive = !memory::IsReferenceType(arr_type);
    *load_obj = (!is_primitive) && (value != 0);

    return value;
}

ALWAYS_INLINE void HandleStoreToArray(int64_t array_ptr, int64_t array_idx, int64_t src_reg_value)
{
    auto *array = reinterpret_cast<types::Array *>(array_ptr);
    assert(array != nullptr);

    auto *gc = runtime::Runtime::GetInstance()->GetGC();
    auto arr_type = array->GetClassWord()->GetArrayElementType();
    auto arr_mark_word = array->GetMarkWord();

    if (memory::IsReferenceType(arr_type) && src_reg_value != 0) {
        auto *obj_ptr = reinterpret_cast<ObjectHeader *>(src_reg_value);
        if (obj_ptr->GetMarkWord().mark == 0 && arr_mark_word.mark == 1 && arr_mark_word.neighbour == 1) {
            array->SetMarkWord({.mark = 1, .neighbour = 0});
            gc->AddGreyObject(array);
        }
    }

    PrintLog("array_ptr = ", (void *)array_ptr, ", idx = ", array_idx, ", src_reg_value = ", src_reg_value);
    array->Set(src_reg_value, array_idx);
}

ALWAYS_INLINE int64_t HandleCreateStringObject(int32_t string_offset)
{
    auto *runtime = Runtime::GetInstance();

    const std::string *string = runtime->GetStringFromCache(string_offset);
    if (string == nullptr) {
        string = runtime->CreateStringAndSetInCache(string_offset);
    }

    auto *class_description =
        runtime->GetClassManager()->GetDefaultClassDescription(ClassManager::DefaultClassDescr::STRING);

    if (UNLIKELY(class_description == nullptr)) {
        PrintErr("ClassDescription for string should be initialized due Runtime creation");
        UNREACHABLE();
    }
    assert(class_description->IsStringObject());

    // string->size + 1 because of \0 at the end of c_string
    auto *string_obj = types::String::Create(reinterpret_cast<const uint8_t *>(string->c_str()), string->size() + 1);
    if (UNLIKELY(string_obj == nullptr)) {
        PrintErr("Error when creating object for string \"", string->c_str(), "\"");
        UNREACHABLE();
    }

    string_obj->SetClassWord(class_description);

    return reinterpret_cast<int64_t>(string_obj);
}

ALWAYS_INLINE void HandlePrintString(int64_t string_ptr)
{
    auto *string = reinterpret_cast<types::String *>(string_ptr);
    printf("%s\n", string->GetData());
}

ALWAYS_INLINE int64_t HandleStringConcatenation(int64_t lhs_string, int64_t rhs_string)
{
    return reinterpret_cast<int64_t>(types::String::ConcatStrings(reinterpret_cast<types::String *>(lhs_string),
                                                                  reinterpret_cast<types::String *>(rhs_string)));
}

ALWAYS_INLINE int64_t HandleStringComparison(int64_t lhs_string, int64_t rhs_string)
{
    return types::String::CompareStrings(reinterpret_cast<types::String *>(lhs_string),
                                         reinterpret_cast<types::String *>(rhs_string));
}

ALWAYS_INLINE int64_t HandleCreateObject(file_format::File *file, int16_t class_number /* idx in the class table */)
{
    auto *heap_manager = Runtime::GetInstance()->GetHeapManager();
    auto *class_manager = Runtime::GetInstance()->GetClassManager();

    auto *asm_classes = file->GetHeader()->GetClassSection()->GetInstances();
    auto &asm_class = (*asm_classes)[class_number];

    ClassDescription *class_description = nullptr;
    if (class_description = class_manager->GetClassDescriptionFromCache(asm_class.GetName());
        class_description == nullptr) {
        class_description = class_manager->CreateClassDescription(asm_class);
    }

    auto *class_obj = static_cast<types::Class *>(
        heap_manager->AllocateObject(sizeof(ObjectHeader) + class_description->GetClassSize()));
    class_obj->SetClassWord(class_description);
    class_obj->InitFields(asm_class);

    PrintLog("obj_ptr = ", (long)class_obj, ", class_size = ", class_description->GetClassSize());
    return reinterpret_cast<int64_t>(class_obj);
}

// reg_idx -- register in which object field will be set after getting from object
ALWAYS_INLINE int64_t HandleObjGetField(int16_t field_idx, int64_t obj_ptr, bool *load_obj)
{
    auto *cls = reinterpret_cast<types::Class *>(obj_ptr);
    assert(cls != nullptr);

    // int64_t because all existing field types take up 8 bytes
    int64_t raw_field = cls->GetField(static_cast<size_t>(field_idx));
    bool is_primitive = cls->IsFieldPrimitive(static_cast<size_t>(field_idx));
    *load_obj = (!is_primitive) && (raw_field != 0);

    return raw_field;
}

// reg -- register value which will be set to field_idx
ALWAYS_INLINE void HandleObjSetField(int16_t field_idx, int64_t reg, int64_t obj_ptr_val)
{
    auto *cls = reinterpret_cast<types::Class *>(obj_ptr_val);
    assert(cls != nullptr);

    auto *gc = runtime::Runtime::GetInstance()->GetGC();
    auto field_type = cls->GetClassWord()->GetField(field_idx).GetType();
    auto cls_mark_word = cls->GetMarkWord();

    if (memory::IsReferenceType(field_type) && reg != 0) {
        auto *obj_ptr = reinterpret_cast<ObjectHeader *>(reg);
        if (obj_ptr->GetMarkWord().mark == 0 && cls_mark_word.mark == 1 && cls_mark_word.neighbour == 1) {
            cls->SetMarkWord({.mark = 1, .neighbour = 0});
            gc->AddGreyObject(cls);
        }
    }

    PrintLog("obj_ptr = ", (long)cls, ", field_idx = ", field_idx, ", field_type = ", static_cast<int>(field_type));
    cls->SetField(static_cast<size_t>(field_idx), reg);
}

} // namespace evm::runtime

#endif // EVM_RUNTIME_INTERPRETER_INL_H
