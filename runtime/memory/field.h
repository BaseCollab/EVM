#ifndef EVM_RUNTIME_FIELD_H
#define EVM_RUNTIME_FIELD_H

#include "common/macros.h"
#include "runtime/memory/type.h"

namespace evm::runtime {

class Field {
public:
    NO_MOVE_SEMANTIC(Field);
    NO_COPY_SEMANTIC(Field);

    explicit Field(memory::Type type, size_t size, size_t offset) : type_(type), size_(size), offset_(offset) {}

    ~Field() = default;

    bool IsObject() const
    {
        return type_ == memory::Type::CLASS_OBJECT;
    }

    bool IsString() const
    {
        return type_ == memory::Type::STRING_OBJECT;
    }

    bool IsArray() const
    {
        return type_ == memory::Type::ARRAY_OBJECT;
    }

    bool IsPrimitive() const
    {
        return !IsObject() && !IsString() && !IsArray();
    }

    size_t GetOffset() const
    {
        return offset_;
    }

    size_t GetSize() const
    {
        return size_;
    }

    memory::Type GetType() const
    {
        return type_;
    }

private:
    memory::Type type_ {memory::Type::INVALID};

    size_t size_ {0};
    size_t offset_ {0};
};

} // namespace evm::runtime

#endif // EVM_RUNTIME_FIELD_H
