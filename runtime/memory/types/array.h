#ifndef EVM_MEMORY_OBJECTS_ARRAY_H
#define EVM_MEMORY_OBJECTS_ARRAY_H

#include "common/macros.h"
#include "runtime/memory/type.h"

#include <cstddef>
#include <cstring>
#include <string_view>

namespace evm::runtime::types {

class Array {
public:
    NO_COPY_SEMANTIC(Array);
    NO_MOVE_SEMANTIC(Array);

    Array() = default;
    ~Array() = default;

    static void *Create(memory::Type array_type, size_t count);

    const void *GetData() const
    {
        return data_;
    }

    void SetData(void *data_ptr)
    {
        data_ = data_ptr;
    }

    size_t GetSize() const
    {
        return size_;
    }

    memory::Type GetType() const
    {
        return type_;
    }

    void SetSize(size_t size)
    {
        size_ = size;
    }

    void SetType(memory::Type type)
    {
        type_ = type;
    }

    void SetElemSize(size_t elem_size)
    {
        elem_size_ = elem_size;
    }

    void Set(int64_t value, size_t idx);

    void Get(int64_t *value, size_t idx) const;

    static constexpr uint32_t GetDataOffset()
    {
        return MEMBER_OFFSET(Array, data_);
    }

private:
    memory::Type type_ {memory::Type::INVALID};
    size_t elem_size_ {0};
    size_t size_ {0};

    void *data_ {nullptr};
};

} // namespace evm::runtime::types

#endif // EVM_MEMORY_OBJECTS_ARRAY_H
