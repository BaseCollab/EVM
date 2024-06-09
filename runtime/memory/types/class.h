#ifndef EVM_RUNTIME_MEMORY_TYPES_CLASS
#define EVM_RUNTIME_MEMORY_TYPES_CLASS

#include "runtime/memory/object_header.h"
#include "file_format/file.h"

#include <cstddef>

namespace evm::runtime::types {

class Class : public ObjectHeader {
public:
    NO_COPY_SEMANTIC(Class);
    NO_MOVE_SEMANTIC(Class);

    Class() = default;
    ~Class() = default;

    static constexpr uint32_t GetDataOffset()
    {
        return MEMBER_OFFSET(Class, data_);
    }

    void InitFields(file_format::Class &asm_class);

    int64_t GetField(size_t field_idx, memory::Type field_type);
    void SetField(size_t field_idx, memory::Type field_type, int64_t data);

    bool FieldIsPrimitive(size_t field_idx);

private:
    __extension__ uint8_t data_[0];
};

} // namespace evm::runtime::types

#endif // EVM_RUNTIME_MEMORY_TYPES_CLASS
