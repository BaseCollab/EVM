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
    NO_MOVE_SEMANTIC(ClassManager);
    NO_COPY_SEMANTIC(ClassManager);

    ClassManager() = default;
    ~ClassManager() = default;

    ClassDescription *GetClassDescriptionFromCache(const std::string &class_name);
    ClassDescription *CreateClassDescription(file_format::Class &asm_class);

private:
    std::pair<Field *, size_t> CreateFields(file_format::Class &asm_class);

private:
    std::unordered_map<std::string, ClassDescription *> class_description_cache_;
};

} // namespace evm::runtime

#endif // EVM_MEMORY_CLASS_MANAGER_H
