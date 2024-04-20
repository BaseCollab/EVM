#include "bump_allocator.h"
#include "memory/types/array.h"

#include <iostream>
#include <cassert>

namespace evm::memory {

BumpAllocator::BumpAllocator(uint8_t *heap, size_t heap_capacity) : 
    AllocatorBase(heap, heap_capacity),
    next_alloc_(heap_)
{}

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

} // namespace evm::memory
