#include "runtime/runtime.h"
#include "runtime/memory/field.h"
#include "runtime/memory/class_manager.h"
#include "runtime/memory/class_description.h"
#include "file_format/file.h"

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
    auto *classDescr = static_cast<ClassDescription *>(heap_manager->AllocateInternalObject(sizeof(ClassDescription)));

    auto [fields, fields_size] = CreateFields(asm_class);
    classDescr->SetFields(fields);
    classDescr->SetFieldsNum(fields_size);
    classDescr->SetObjectType(memory::Type::CLASS_OBJECT);

    class_description_cache_.insert({asm_class.GetName(), classDescr});

    return classDescr;
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

        printf("[ClassManager::CreateFields] relative_offset = %ld, field_size = %ld, type = %d\n", relative_offset,
               size, static_cast<int8_t>(type));

        new (&runtime_fields[idx]) Field(type, size, relative_offset);
    }

    return std::make_pair(runtime_fields, fields_num);
}

void ClassManager::InitDefaultClassDescriptions()
{
    CreateDefaultClassDescription(DefaultClassDescr::INT_ARRAY);
    CreateDefaultClassDescription(DefaultClassDescr::DOUBLE_ARRAY);
    CreateDefaultClassDescription(DefaultClassDescr::STRING);
}

void ClassManager::CreateDefaultClassDescription(DefaultClassDescr default_class_descr_type)
{
    auto *heap_manager = Runtime::GetInstance()->GetHeapManager();
    auto *classDescr = static_cast<ClassDescription *>(heap_manager->AllocateInternalObject(sizeof(ClassDescription)));
    assert(classDescr != nullptr);

    switch (default_class_descr_type) {
        case DefaultClassDescr::INT_ARRAY:
        case DefaultClassDescr::DOUBLE_ARRAY:
            classDescr->SetObjectType(memory::Type::ARRAY_OBJECT);
            break;

        case DefaultClassDescr::STRING:
            classDescr->SetObjectType(memory::Type::STRING_OBJECT);
            break;

        default:
            UNREACHABLE();
    }

    assert(static_cast<size_t>(default_class_descr_type) - 1 < default_class_descriptions_.size());
    default_class_descriptions_[static_cast<size_t>(default_class_descr_type) - 1] = classDescr;
}

} // namespace evm::runtime
