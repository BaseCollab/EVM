#ifndef EVM_MEMORY_CLASS_STRUCTURE_H
#define EVM_MEMORY_CLASS_STRUCTURE_H

#include "common/macros.h"

namespace evm {

class ClassStructure;

class Field {
public:

private:
    ClassStructure *class_word_ {nullptr};
    uint32_t offset {0};
};

class ClassStructure {
public:
    NO_COPY_SEMANTIC(ClassStructure);
    NO_MOVE_SEMANTIC(ClassStructure);

    explicit ClassStructure() = default;
    ~ClassStructure() = default;

    memory::Array::Type GetArrayType() const
    {
        assert(element_type_ != nullptr);
        element_type_->
    }

private:
    std::vector<Field> fields_;

    ClassStructure *element_type_ {nullptr};
};

} // namespace evm


#endif // EVM_MEMORY_CLASS_STRUCTURE_H
