#ifndef EVM_MEMORY_OBJECTS_ARRAY_H
#define EVM_MEMORY_OBJECTS_ARRAY_H

#include "common/macros.h"
#include "runtime/memory/type.h"
#include "runtime/memory/object_header.h"
#include "runtime/memory/class_manager.h"

#include <cstddef>
#include <cstring>
#include <string_view>

namespace evm::runtime::types {

class Array : public ObjectHeader {
public:
    NO_COPY_SEMANTIC(Array);
    NO_MOVE_SEMANTIC(Array);

    static Array *Create(memory::Type array_type, size_t length);

    void Set(int64_t value, size_t idx);

    void Get(int64_t *value, size_t idx) const;

    void SetLength(uint32_t length)
    {
        length_ = length;
    }

    size_t GetLength() const
    {
        return length_;
    }

    static constexpr uint32_t GetDataOffset()
    {
        return MEMBER_OFFSET(Array, data_);
    }

private:
    Array() = default;
    ~Array() = default;

    static ClassManager::DefaultClassDescr GetDefaultClassDescrFromType(memory::Type array_type);
    void ValidateAddressingInArray(size_t idx) const;

private:
    size_t length_ {0};
    __extension__ uint8_t data_[0];
};

} // namespace evm::runtime::types

#endif // EVM_MEMORY_OBJECTS_ARRAY_H
