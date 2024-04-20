#ifndef EVM_MEMORY_TYPES_STRING
#define EVM_MEMORY_TYPES_STRING

#include "common/macros.h"
#include "memory/object_header.h"
#include <cstddef>

namespace evm {
class VirtualMachine;
};

namespace evm::memory {

class String : public ObjectHeader {
public:
    static String *Create(const uint8_t *data, size_t length, VirtualMachine *vm);

    static uint32_t CalculateStringHash(const uint8_t *data, size_t length);

    static bool CompareStrings(String *str1, String *str2);

    static bool ConcatString(String *str1, String *str2);

    uint8_t *GetData() const
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

    void SetLength(uint32_t length)
    {
        length_ = length;
    }

    bool IsEmpty() const
    {
        return length_ == 0;
    }

private:
    NO_COPY_SEMANTIC(String);
    NO_MOVE_SEMANTIC(String);

    String() = default;
    ~String() = default;

private:
    size_t length_ {0};
    uint32_t hash_ {0};
    // Used a zero-length array to have a convenient offset for string data
    // and conveniently get a pointer to string data.
    __extension__ uint8_t data_[0];
};

} // namespace evm::memory

#endif // EVM_MEMORY_TYPES_STRING
