#include "runtime/runtime.h"
#include "runtime/memory/field.h"
#include "runtime/memory/class_manager.h"
#include "runtime/memory/class_description.h"
#include "file_format/file.h"

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

        new (&runtime_fields[idx]) Field(type, size, relative_offset);
    }

    return std::make_pair(runtime_fields, fields_num);
}

} // namespace evm::runtime
