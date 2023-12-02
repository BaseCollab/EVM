#include "bump_allocator.h"

#include <iostream>
#include <cassert>

namespace evm::memory {

BumpAllocator::BumpAllocator(size_t heap_capacity) : heap_capacity_(heap_capacity)
{
    if (heap_capacity <= 0) {
        std::cerr << "Invalid heap size, can not allocate heap" << std::endl;
        return;
    }

    heap_ = new uint8_t[heap_capacity_];
    assert(heap_ && "Can not allocate heap");

    next_alloc_ = heap_;
}

/* override */
BumpAllocator::~BumpAllocator()
{
    delete[] heap_;
}

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

/* override */
Array *BumpAllocator::AllocateArray(size_t size)
{
    size_t array_class_size = size + Array::GetDataOffset();
    return reinterpret_cast<Array *>(Alloc(size));
}

} // namespace evm::memory
