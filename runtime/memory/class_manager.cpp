#include "runtime/runtime.h"
#include "runtime/memory/field.h"
#include "runtime/memory/class_manager.h"
#include "runtime/memory/class_description.h"
#include "file_format/class_section.h"

#include <cassert>

namespace evm::runtime {

ClassDescription *ClassManager::GetClassDescriptionFromCache(const std::string &class_name)
{
    if (auto find = class_description_cache_.find(class_name); find != class_description_cache_.end()) {
        return find->second;
    }
    return nullptr;
}

ClassDescription *ClassManager::CreateClassDescription(file_format::Class &asm_class)
{
    auto *heap_manager = Runtime::GetInstance()->GetHeapManager();
    auto *class_descr = static_cast<ClassDescription *>(heap_manager->AllocateInternalObject(sizeof(ClassDescription)));

    auto [fields, fields_size] = CreateFields(asm_class);
    class_descr->SetFields(fields);
    class_descr->SetFieldsNum(fields_size);
    class_descr->SetObjectType(memory::Type::CLASS_OBJECT);

    class_description_cache_.insert({asm_class.GetName(), class_descr});

    return class_descr;
}

std::pair<Field *, size_t> ClassManager::CreateFields(file_format::Class &asm_class)
{
    auto *heap_manager = Runtime::GetInstance()->GetHeapManager();

    auto *asm_fields = asm_class.GetInstances();
    size_t fields_num = asm_fields->size();

    auto *runtime_fields = static_cast<Field *>(heap_manager->AllocateInternalObject(fields_num * sizeof(Field)));

    for (size_t idx = 0; idx < fields_num; ++idx) {
        auto &current_asm_field = (*asm_fields)[idx];

        auto [relative_offset, size] = asm_class.GetRuntimeOffsetOfInstance(current_asm_field.GetName());
        auto type = current_asm_field.GetType();

        PrintLog("relative_offset =  = ", relative_offset, "field_size = ", size, ", type = ", static_cast<int8_t>(type));

        new (&runtime_fields[idx]) Field(type, size, relative_offset);
    }

    return std::make_pair(runtime_fields, fields_num);
}

void ClassManager::InitDefaultClassDescriptions()
{
    CreateDefaultClassDescription(DefaultClassDescr::INT_ARRAY);
    CreateDefaultClassDescription(DefaultClassDescr::DOUBLE_ARRAY);
    CreateDefaultClassDescription(DefaultClassDescr::OBJECT_ARRAY);
    CreateDefaultClassDescription(DefaultClassDescr::STRING);
}

void ClassManager::CreateDefaultClassDescription(DefaultClassDescr default_class_descr_type)
{
    auto *heap_manager = Runtime::GetInstance()->GetHeapManager();
    auto *class_descr = static_cast<ClassDescription *>(heap_manager->AllocateInternalObject(sizeof(ClassDescription)));
    assert(class_descr != nullptr);

    switch (default_class_descr_type) {
        case DefaultClassDescr::INT_ARRAY:
            class_descr->SetObjectType(memory::Type::ARRAY_OBJECT);
            class_descr->SetArrayElementType(memory::Type::INT);
            break;

        case DefaultClassDescr::DOUBLE_ARRAY:
            class_descr->SetObjectType(memory::Type::ARRAY_OBJECT);
            class_descr->SetArrayElementType(memory::Type::DOUBLE);
            break;

        case DefaultClassDescr::OBJECT_ARRAY:
            class_descr->SetObjectType(memory::Type::ARRAY_OBJECT);
            break;

        case DefaultClassDescr::STRING:
            class_descr->SetObjectType(memory::Type::STRING_OBJECT);
            break;

        default:
            UNREACHABLE();
    }

    assert(static_cast<size_t>(default_class_descr_type) - 1 < default_class_descriptions_.size());
    default_class_descriptions_[static_cast<size_t>(default_class_descr_type) - 1] = class_descr;
}

} // namespace evm::runtime
