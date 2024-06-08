#ifndef EVM_RUNTIME_MEMORY_OBJECT_HEADER_H
#define EVM_RUNTIME_MEMORY_OBJECT_HEADER_H

#include "common/macros.h"
#include "mark_word.h"
#include "runtime/memory/class_description.h"

namespace evm::runtime {

class ObjectHeader {
public:
    NO_COPY_SEMANTIC(ObjectHeader);
    NO_MOVE_SEMANTIC(ObjectHeader);

    ObjectHeader() = default;
    ~ObjectHeader() = default;

    MarkWord GetMarkWord() const
    {
        return mark_word_;
    }

    void SetMarkWord(MarkWord mark_word)
    {
        mark_word_ = mark_word;
    }

    void SetClassWord(ClassDescription *class_word)
    {
        class_word_ = class_word;
    }

    ClassDescription *GetClassWord()
    {
        return class_word_;
    }

    const ClassDescription *GetClassWord() const
    {
        return class_word_;
    }

private:
    MarkWord mark_word_;
    ClassDescription *class_word_ {nullptr};
};

} // namespace evm::runtime

#endif // EVM_RUNTIME_MEMORY_OBJECT_HEADER_H
