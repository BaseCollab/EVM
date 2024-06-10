#include "runtime/memory/types/class.h"
#include "runtime/memory/types/array.h"
#include "runtime/runtime.h"
#include "common/utils/bitops.h"

#include <cassert>

namespace evm::runtime::types {

void Class::InitFields(file_format::Class &asm_class)
{
    // printf("Init fields start, class = %s\n", asm_class.GetName().c_str());
    auto *heap_manager = Runtime::GetInstance()->GetHeapManager();
    auto *class_manager = Runtime::GetInstance()->GetClassManager();
    auto *file = Runtime::GetInstance()->GetExecutableFile();

    auto *asm_fields = asm_class.GetInstances();

    for (size_t idx = 0; idx < asm_fields->size(); ++idx) {
        auto &current_asm_field = (*asm_fields)[idx];
        if (current_asm_field.IsClassObject()) {
            auto &field_asm_class = file->GetClassFromClassSection(current_asm_field.GetClassRefIdx());
            // printf("field %s, name of class %s\n", current_asm_field.GetName().c_str(),
            //        field_asm_class.GetName().c_str());

            auto *class_description = class_manager->GetClassDescriptionFromCache(field_asm_class.GetName());
            if (class_description == nullptr) {
                class_description = class_manager->CreateClassDescription(field_asm_class);
            }

            auto *class_obj = static_cast<types::Class *>(
                heap_manager->AllocateObject(sizeof(ObjectHeader) + class_description->GetClassSize()));
            class_obj->SetClassWord(class_description);
            class_obj->InitFields(field_asm_class);

            SetField(idx, bitops::BitCast<int64_t>(class_obj));
        } else if (current_asm_field.IsArrayObject()) {
            printf("Array_size = %ld\n", current_asm_field.GetArraySize());

            auto element_type = current_asm_field.GetArrayElementType();

            auto *array_obj = types::Array::Create(element_type, current_asm_field.GetArraySize());

            if (IsReferenceType(element_type)) {
                // In case of reference default class description don't set element type
                array_obj->GetClassWord()->SetArrayElementType(element_type);
            }

            SetField(idx, bitops::BitCast<int64_t>(array_obj));
        }
    }
    // printf("Init fields end, class = %s\n", asm_class.GetName().c_str());
}

bool Class::FieldIsPrimitive(size_t field_idx)
{
    const Field &field = GetClassWord()->GetField(field_idx);
    return field.IsPrimitive();
}

int64_t Class::GetField(size_t field_idx)
{
    const Field &field = GetClassWord()->GetField(field_idx);
    auto offset = field.GetOffset();

    int64_t raw_field = 0;
    uint8_t *field_ptr = reinterpret_cast<uint8_t *>(this) + GetDataOffset() + offset;

    std::memcpy(&raw_field, field_ptr, sizeof(raw_field));
    return raw_field;
}

void Class::SetField(size_t field_idx, int64_t data)
{
    printf("[Class::SetField] field_idx = %ld\n", field_idx);
    auto offset = GetClassWord()->GetField(field_idx).GetOffset();
    uint8_t *field_ptr = reinterpret_cast<uint8_t *>(this) + GetDataOffset() + offset;

    std::memcpy(field_ptr, &data, sizeof(data));
}

} // namespace evm::runtime::types
