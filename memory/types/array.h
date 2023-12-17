#ifndef EVM_MEMORY_OBJECTS_ARRAY_H
#define EVM_MEMORY_OBJECTS_ARRAY_H

#include "common/macros.h"

#include "vm/vm.h"

#include <cstddef>
#include <cstring>

namespace evm::memory {

class Array {
public:
    // clang-format off
    enum class Type {
        INVALID = -1,
        DOUBLE  = 2,
        BYTE    = 3,
        SHORT   = 4,
        INT     = 5,
        LONG    = 6
    };
    // clang-format on
public:
    NO_COPY_SEMANTIC(Array);
    NO_MOVE_SEMANTIC(Array);

    Array() = default;
    ~Array() = default;

    static void *Create(const VirtualMachine &vm, Type array_type, size_t count);

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

    Type GetType() const
    {
        return type_;
    }

    void SetSize(size_t size)
    {
        size_ = size;
    }

    void SetType(Type type)
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

    static size_t GetSizeOfArrayType(Type array_type);
    static Type GetTypeFromString(std::string_view string);

private:
    Type type_ {Type::INVALID};
    size_t elem_size_ {0};
    size_t size_ {0};

    void *data_ {nullptr};
};

} // namespace evm::memory

#endif // EVM_MEMORY_OBJECTS_ARRAY_H
