#include "runtime/memory/heap_manager.h"
#include "runtime/memory/allocator/bump_allocator.h"

#include <sys/mman.h>
#include <iostream>

namespace evm::runtime {

HeapManager::HeapManager(size_t size) : heap_size_(size)
{
    heap_ =
        static_cast<uint8_t *>(mmap(nullptr, heap_size_, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    if (heap_ == nullptr) {
        std::cerr << "HeapManager::Failed to mmap heap, errno = " << errno << std::endl;
        return;
    }

    object_allocator_ = std::make_unique<BumpAllocator>(heap_, heap_size_);
}

HeapManager::~HeapManager()
{
    int success = munmap(heap_, heap_size_);
    if (success == -1) {
        std::cerr << "HeapManager::Errors in munmap, errno = " << errno << std::endl;
    }
}

const std::vector<ObjectHeader *> &HeapManager::GetObjectsList() const
{
    return objects_;
}

void *HeapManager::AllocateObject(size_t size)
{
    void *alloc_obj = object_allocator_->Alloc(size);
    objects_.push_back(reinterpret_cast<ObjectHeader *>(alloc_obj));
    return alloc_obj;
}

void *HeapManager::AllocateInternalObject(size_t size)
{
    return object_allocator_->Alloc(size);
}

} // namespace evm::runtime
