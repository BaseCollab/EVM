#ifndef EVM_RUNTIME_MEMORY_ALLOCATOR_BUMP_ALLOCATOR_H
#define EVM_RUNTIME_MEMORY_ALLOCATOR_BUMP_ALLOCATOR_H

#include "allocator.h"

#include <cstddef>
#include <cstdint>

namespace evm::runtime {

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

    explicit BumpAllocator(uint8_t *heap, size_t heap_capacity)
        : heap_(heap), next_alloc_(heap_), heap_capacity_(heap_capacity)
    {
    }
    ~BumpAllocator() override = default;

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

} // namespace evm::runtime

#endif // EVM_RUNTIME_MEMORY_ALLOCATOR_BUMP_ALLOCATOR_H
