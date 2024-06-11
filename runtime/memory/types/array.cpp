#include "common/logs.h"
#include "runtime/memory/types/array.h"
#include "runtime/memory/type.h"
#include "runtime/runtime.h"

#include <cassert>

namespace evm::runtime::types {

/* static */
Array *Array::Create(memory::Type array_type, size_t length)
{
    auto *runtime = Runtime::GetInstance();

    PrintLog("Array_type = ", static_cast<int>(array_type));

    size_t elem_size = memory::GetSizeOfType(array_type);
    size_t array_size = Array::GetDataOffset() + length * elem_size;

    auto *array_obj = reinterpret_cast<Array *>(runtime->GetHeapManager()->AllocateObject(array_size));
    if (UNLIKELY(array_obj == nullptr)) {
        PrintErr("Error when creating object for array of type \"", GetStringFromType(array_type).c_str(), "\"");
        UNREACHABLE();
    }
    array_obj->SetLength(length);

    auto classDescrType = GetDefaultClassDescrFromType(array_type);
    auto *class_description = runtime->GetClassManager()->GetDefaultClassDescription(classDescrType);
    if (UNLIKELY(class_description == nullptr)) {
        PrintErr("ClassDescription for array should be initialized due Runtime creation");
        UNREACHABLE();
    }

    assert(class_description->IsArrayObject());
    array_obj->SetClassWord(class_description);

    return array_obj;
}

/* static */
ClassManager::DefaultClassDescr Array::GetDefaultClassDescrFromType(memory::Type array_type)
{
    switch (array_type) {
        case memory::Type::DOUBLE:
            return ClassManager::DefaultClassDescr::DOUBLE_ARRAY;

        case memory::Type::INT:
            return ClassManager::DefaultClassDescr::INT_ARRAY;

        case memory::Type::CLASS_OBJECT:
        case memory::Type::STRING_OBJECT:
        case memory::Type::ARRAY_OBJECT:
            return ClassManager::DefaultClassDescr::OBJECT_ARRAY;

        default:
            UNREACHABLE();
    }
}

void Array::ValidateAddressingInArray(size_t idx) const
{
    if (idx >= length_) {
        PrintErr("Get by invalid idx = ", idx, " in array of length ", length_);
        UNREACHABLE();
    }
}

void Array::Set(int64_t value, size_t idx)
{
    ValidateAddressingInArray(idx);

    auto array_type = GetClassWord()->GetArrayElementType();
    if (UNLIKELY(array_type == memory::Type::INVALID)) {
        PrintErr("Array type is INVALID in array object header");
        UNREACHABLE();
    }

    size_t elem_size = memory::GetSizeOfType(array_type);

    uint8_t *data = reinterpret_cast<uint8_t *>(this) + Array::GetDataOffset();

    std::memcpy(data + idx * elem_size, &value, elem_size * sizeof(uint8_t));
}

void Array::Get(int64_t *value, size_t idx) const
{
    ValidateAddressingInArray(idx);

    auto array_type = GetClassWord()->GetArrayElementType();
    if (UNLIKELY(array_type == memory::Type::INVALID)) {
        PrintErr("Array type is INVALID in array object header");
        UNREACHABLE();
    }

    size_t elem_size = memory::GetSizeOfType(array_type);
    const uint8_t *data = reinterpret_cast<const uint8_t *>(this) + Array::GetDataOffset();

    std::memcpy(value, data + idx * elem_size, elem_size * sizeof(uint8_t));
}

} // namespace evm::runtime::types
