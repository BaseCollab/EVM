#include "common/logs.h"
#include "runtime/memory/heap_manager.h"
#include "runtime/memory/allocator/freelist_allocator.h"

#include <sys/mman.h>

namespace evm::runtime {

HeapManager::HeapManager(size_t size) : heap_size_(size)
{
    heap_ =
        static_cast<uint8_t *>(mmap(nullptr, heap_size_, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    if (heap_ == nullptr) {
        PrintErr("Failed to mmap heap, errno = ", errno);
        return;
    }

    object_allocator_ = std::make_unique<FreelistAllocator>(heap_, heap_size_);
}

HeapManager::~HeapManager()
{
    int success = munmap(heap_, heap_size_);
    if (success == -1) {
        PrintErr("Errors in munmap, errno = ", errno);
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

void HeapManager::DeallocateObject(void *obj_ptr)
{
    object_allocator_->Dealloc(obj_ptr);
}

void *HeapManager::AllocateInternalObject(size_t size)
{
    return object_allocator_->Alloc(size);
}

} // namespace evm::runtime
