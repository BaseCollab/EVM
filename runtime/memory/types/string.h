#ifndef EVM_RUNTIME_MEMORY_TYPES_STRING
#define EVM_RUNTIME_MEMORY_TYPES_STRING

#include "runtime/memory/object_header.h"

#include <cstddef>

namespace evm::runtime {

class String : public ObjectHeader {
public:
    NO_COPY_SEMANTIC(String);
    NO_MOVE_SEMANTIC(String);

    static String *Create(const uint8_t *data, size_t length);

    static uint32_t CalculateStringHash(const uint8_t *data, size_t length);

    static int CompareStrings(String *str1, String *str2);

    static String *ConcatStrings(String *lhs_string, String *rhs_string);

    uint8_t *GetData()
    {
        return data_;
    }

    const uint8_t *GetData() const
    {
        return data_;
    }

    static constexpr uint32_t GetDataOffset()
    {
        return MEMBER_OFFSET(String, data_);
    }

    void SetHash(uint32_t hash)
    {
        hash_ = hash;
    }

    uint32_t GetHash() const
    {
        return hash_;
    }

    void SetLength(uint32_t length)
    {
        length_ = length;
    }

    size_t GetLength() const
    {
        return length_;
    }

    bool IsEmpty() const
    {
        return length_ == 0;
    }

private:
    String() = default;
    ~String() = default;

private:
    size_t length_ {0};
    uint32_t hash_ {0};
    // Used a zero-length array to have a convenient offset for string data
    // and conveniently get a pointer to string data.
    __extension__ uint8_t data_[0];
};

} // namespace evm::runtime

#endif // EVM_RUNTIME_MEMORY_TYPES_STRING
