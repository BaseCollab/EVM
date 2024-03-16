#ifndef EVM_MEMORY_ALLOCATOR_H
#define EVM_MEMORY_ALLOCATOR_H

#include "common/macros.h"

#include <cstddef>
#include <memory>

namespace evm::memory {

class AllocatorBase {
public:
    enum class AllocatorType {
        INVALID = -1,
        BUMP = 0,
        FREE_LIST = 1,
    };

public:
    NO_COPY_SEMANTIC(AllocatorBase);
    NO_MOVE_SEMANTIC(AllocatorBase);

    explicit AllocatorBase() = default;
    virtual ~AllocatorBase() = default;

    AllocatorType GetAllocatorType() const
    {
        return allocator_type_;
    }

    virtual void *Alloc(size_t size) = 0;

    virtual size_t GetHeapCapacity() const = 0;

private:
    AllocatorType allocator_type_ {AllocatorType::BUMP};

    void *heap_ {nullptr};
    size_t heap_size_ {0};
};

} // namespace evm::memory

#endif // EVM_MEMORY_ALLOCATOR_H
