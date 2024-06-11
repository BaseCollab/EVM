#include "common/logs.h"
#include "bump_allocator.h"

#include <cstddef>

namespace evm::runtime {

/* override */
void *BumpAllocator::Alloc(size_t size)
{
    if (size > heap_capacity_ - busy_size_) {
        PrintErr("BumpAllocator::Alloc failed, size is too big");
        return nullptr;
    }

    void *alloc_ptr = next_alloc_;

    next_alloc_ += size;
    busy_size_ += size;

    return alloc_ptr;
}

} // namespace evm::runtime
