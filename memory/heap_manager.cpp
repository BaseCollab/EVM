#include "heap_manager.h"


#include <sys/mman.h>
#include <iostream>

namespace evm::memory {
    
HeapManager::HeapManager(size_t size) :
    heap_size_(heap_size_)
{
    heap_ = static_cast<uint8_t *>(mmap(nullptr, heap_size_, 0, 0, -1, 0));
    if (heap_ == nullptr) {
        std::cerr << "Failed to mmap heap" << std::endl;
        return;
    }

    objectAllocator_ = std::make_unique<BumpAllocator>(heap_, heap_size_);
}

HeapManager::~HeapManager()
{
    int success = munmap(heap_, heap_size_);
    if (success == -1) {
        std::cerr << "Errors in munmap" << std::endl;
    }
}

ObjectHeader *HeapManager::AllocateObject(size_t size)
{
    return reinterpret_cast<ObjectHeader *>(objectAllocator_->Alloc(size));
}

} // namespace evm::memory
