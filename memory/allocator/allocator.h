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

    explicit AllocatorBase(uint8_t *heap, size_t heap_capacity) :
        heap_(heap),
        heap_capacity_(heap_capacity)
    {}

    virtual ~AllocatorBase() = default;

    AllocatorType GetAllocatorType() const
    {
        return allocator_type_;
    }

    size_t GetHeapCapacity() const
    {
        return heap_capacity_;
    }

    virtual void *Alloc(size_t size) = 0;

protected:
    AllocatorType allocator_type_ {AllocatorType::INVALID};

    uint8_t *heap_ {nullptr};
    size_t heap_capacity_ {0};
};

} // namespace evm::memory

#endif // EVM_MEMORY_ALLOCATOR_H
