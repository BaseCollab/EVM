#ifndef EVM_MEMORY_CLASS_MANAGER_H
#define EVM_MEMORY_CLASS_MANAGER_H

#include "common/macros.h"
#include "file_format/class_section.h"

#include <unordered_map>
#include <utility>

namespace evm::runtime {

class ClassDescription;
class Field;

class ClassManager {
public:
    // clang-format off
    enum class DefaultClassDescr : uint8_t {
        INVALID      = 0,
        INT_ARRAY    = 1,
        DOUBLE_ARRAY = 2,
        STRING       = 3, // should be last
    };
    // clang-format on
public:
    NO_MOVE_SEMANTIC(ClassManager);
    NO_COPY_SEMANTIC(ClassManager);

    ClassManager() : default_class_descriptions_(DefaultClassDescriptionsNumber()) {}
    ~ClassManager() = default;

    ClassDescription *GetClassDescriptionFromCache(const std::string &class_name);
    ClassDescription *CreateClassDescription(file_format::Class &asm_class);

    void InitDefaultClassDescriptions();

    ClassDescription *GetDefaultClassDescription(DefaultClassDescr default_class_descr_type)
    {
        size_t idx = static_cast<size_t>(default_class_descr_type) - 1;
        if (idx < default_class_descriptions_.size()) {
            return default_class_descriptions_[idx];
        }
        return nullptr;
    }

private:
    std::pair<Field *, size_t> CreateFields(file_format::Class &asm_class);

    static constexpr size_t DefaultClassDescriptionsNumber()
    {
        return static_cast<size_t>(DefaultClassDescr::STRING);
    }

    void CreateDefaultClassDescription(DefaultClassDescr default_class_descr_type);

private:
    std::unordered_map<std::string, ClassDescription *> class_description_cache_;
    std::vector<ClassDescription *> default_class_descriptions_;
};

} // namespace evm::runtime

#endif // EVM_MEMORY_CLASS_MANAGER_H
