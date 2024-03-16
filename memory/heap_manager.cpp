#include "heap_manager.h"

#include <sys/mman.h>
#include <iostream>

namespace evm::memory {
    
HeapManager::HeapManager(size_t size) :
    heap_size_(heap_size_)
{
    heap_ = mmap(nullptr, heap_size_, 0, 0, -1, 0);
    if (heap_ == nullptr) {
        std::cerr << "Failed to mmap heap" << std::endl;
        return;
    }

    objectAllocator_ = std::make_unique<BumbAllocator>();
}

HeapManager::~HeapManager()
{
    int success = munmap(heap_, heap_size_);
    if (success == -1) {
        std::cerr << "Errors in munmap" << std::endl;
    }
}

/* static */
ObjectHeader *AllocateObject(size_t size)
{
}

} // namespace evm::memory
