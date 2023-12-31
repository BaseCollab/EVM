#ifndef EVM_MEMORY_ALLOCATOR_BUMP_ALLOCATOR_H
#define EVM_MEMORY_ALLOCATOR_BUMP_ALLOCATOR_H

#include "allocator.h"

namespace evm::memory {

class Array;

/**
 *  +----------------+-------------------+
 *  |xxxxxxxxxxxxxxxx|                   |
 *  |xxxxxxxxxxxxxxxx|    free memory    |
 *  |xxxxxxxxxxxxxxxx|                   |
 *  +----------------+-------------------+
 *                  ||
 *                  || Bump object
 *                  \/
 *  +----------------------+-------------+
 *  |xxxxxxxxxxxxxxxxxxxxxx|             |
 *  |xxxxxxxxxxxxxxxxxxxxxx| free memory |
 *  |xxxxxxxxxxxxxxxxxxxxxx|             |
 *  +----------------------+-------------+
 *
 */
class BumpAllocator final : public AllocatorBase {
public:
    NO_COPY_SEMANTIC(BumpAllocator);
    NO_MOVE_SEMANTIC(BumpAllocator);

    explicit BumpAllocator(size_t heap_capacity = DEFAULT_HEAP_SIZE);
    ~BumpAllocator() override;

    size_t GetHeapCapacity() const override
    {
        return heap_capacity_;
    }

    size_t GetBusySize() const
    {
        return busy_size_;
    }

    void *Alloc(size_t size) override;

private:
    uint8_t *heap_ {nullptr};
    uint8_t *next_alloc_ {nullptr};

    size_t busy_size_ {0};
    size_t heap_capacity_ {0};
};

} // namespace evm::memory

#endif // EVM_MEMORY_ALLOCATOR_BUMP_ALLOCATOR_H
