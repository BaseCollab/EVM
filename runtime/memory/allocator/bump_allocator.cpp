#include "bump_allocator.h"
#include "runtime/memory/types/array.h"

#include <iostream>
#include <cassert>

namespace evm::runtime {

/* override */
void *BumpAllocator::Alloc(size_t size)
{
    if (size > heap_capacity_ - busy_size_) {
        std::cerr << "BumpAllocator::Alloc failed, size is too big" << std::endl;
        return nullptr;
    }

    void *alloc_ptr = next_alloc_;

    next_alloc_ += size;
    busy_size_ += size;

    return alloc_ptr;
}

} // namespace evm::runtime
