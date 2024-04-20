#ifndef EVM_MEMORY_OBJECTS_ARRAY_H
#define EVM_MEMORY_OBJECTS_ARRAY_H

#include "common/macros.h"
#include "memory/object_header.h"

#include <cstddef>
#include <cstdint>

namespace evm {
class VirtualMachine;
};

namespace evm::memory {

class Array : public ObjectHeader {
public:
    // clang-format off
    enum class Type {
        INVALID = -1,
        DOUBLE  = 2,
        BYTE    = 3,
        INT     = 4,
    };
    // clang-format on
public:
    static Array *Create(uint8_t *data, size_t length, Type type, VirtualMachine *vm);

    template<typename T>
    void SetPrimitive(T value, size_t idx);

    template<typename T>
    void GetPrimitive(T *value, size_t idx) const;

    uint8_t *GetData() const
    {
        return data_;
    }

    size_t GetLength() const
    {
        return length_;
    }

    void SetLength(size_t length)
    {
        length_ = length;
    }

    static constexpr uint32_t GetDataOffset()
    {
        return MEMBER_OFFSET(Array, data_);
    }

    static size_t GetSizeOfArrayType(Array::Type array_type);
    
    static Type GetTypeFromString(std::string_view string);

private:
    NO_COPY_SEMANTIC(Array);
    NO_MOVE_SEMANTIC(Array);

    Array() = default;
    ~Array() = default;

private:
    size_t length_ {0};
    uint8_t data_[0];
};

} // namespace evm::memory

#endif // EVM_MEMORY_OBJECTS_ARRAY_H
