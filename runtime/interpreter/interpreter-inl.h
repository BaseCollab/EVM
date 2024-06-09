#ifndef EVM_RUNTIME_INTERPRETER_INL_H
#define EVM_RUNTIME_INTERPRETER_INL_H

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
        printf("[HandleCreateStringObject] Error when creating object for array of type \"%s\"\n",
               GetStringFromType(array_type).c_str());
        UNREACHABLE();
    }

    return reinterpret_cast<int64_t>(array_obj);
}

ALWAYS_INLINE int64_t HandleLoadFromArray(int64_t array_ptr, int64_t idx)
{
    auto *array = reinterpret_cast<types::Array *>(array_ptr);
    int64_t value = 0;
    array->Get(&value, idx);
    return value;
}

ALWAYS_INLINE void HandleStoreToArray(int64_t array_ptr, int64_t array_idx, int64_t src_reg_value)
{
    auto *array = reinterpret_cast<types::Array *>(array_ptr);
    assert(array != nullptr);
    printf("array_ptr = %p, idx = %ld, src_reg_value = %ld\n", (void *)array_ptr, array_idx, src_reg_value);
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
    assert(class_description->IsStringObject());

    if (UNLIKELY(class_description == nullptr)) {
        printf("HandleCreateStringObject::ClassDescription for string should be initialized due Runtime creation\n");
        UNREACHABLE();
    }

    auto *string_obj = types::String::Create(reinterpret_cast<const uint8_t *>(string->data()), string->size());
    if (UNLIKELY(string_obj == nullptr)) {
        printf("HandleCreateStringObject:: Error when creating object for string \"%s\"\n", string->c_str());
        UNREACHABLE();
    }

    string_obj->SetClassWord(class_description);

    return reinterpret_cast<int64_t>(string_obj);
}

ALWAYS_INLINE void HandlePrintString(int64_t string_ptr)
{
    auto *string = reinterpret_cast<types::String *>(string_ptr);
    printf("Print_str = %s\n", string->GetData());
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

    // printf("obj_ptr = %p, class_size = %ld, \n", class_obj, class_description->GetClassSize());
    return reinterpret_cast<int64_t>(class_obj);
}

// reg_idx -- register in which object field will be set after getting from object
ALWAYS_INLINE bool HandleObjGetField(Frame *frame, int16_t field_idx, byte_t reg_idx, byte_t obj_ptr_reg,
                                     int8_t field_type)
{
    auto *cls = reinterpret_cast<types::Class *>(frame->GetReg(obj_ptr_reg)->GetRaw());
    assert(cls != nullptr);

    auto type = static_cast<memory::Type>(field_type);
    printf("obj_ptr = %p, field_idx = %d; field_type = %d\n", static_cast<void *>(cls), field_idx, field_type);
    // int64_t because all existing field types take up 8 bytes
    int64_t raw_field = cls->GetField(static_cast<size_t>(field_idx), type);
    bool is_primitive = cls->FieldIsPrimitive(static_cast<size_t>(field_idx));

    frame->GetReg(reg_idx)->SetInt64(raw_field);

    return is_primitive;
}

// reg_idx -- register from which value will be set to field_idx
ALWAYS_INLINE void HandleObjSetField(Frame *frame, int16_t field_idx, byte_t reg_idx, byte_t obj_ptr_reg,
                                     int8_t field_type)
{
    auto *cls = reinterpret_cast<types::Class *>(frame->GetReg(obj_ptr_reg)->GetRaw());
    assert(cls != nullptr);

    auto type = static_cast<memory::Type>(field_type);
    // printf("obj_ptr = %p, field_idx = %d; field_type = %d\n", cls, field_idx, field_type);
    // int64_t because all existing field types take up 8 bytes
    int64_t data = frame->GetReg(reg_idx)->GetInt64();
    cls->SetField(static_cast<size_t>(field_idx), type, data);
}

} // namespace evm::runtime

#endif // EVM_RUNTIME_INTERPRETER_INL_H
