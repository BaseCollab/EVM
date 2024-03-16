#ifndef MEMORY_HEAP_MANAGER_H
#define MEMORY_HEAP_MANAGER_H

#include "common/macros.h"
#include "allocator/bump_allocator.h"
#include "frame.h"

#include <cstddef>

namespace evm::memory {

class ObjectHeader;

class HeapManager {
public:
    NO_COPY_SEMANTIC(HeapManager);
    NO_MOVE_SEMANTIC(HeapManager);

    explicit HeapManager(size_t heap_size);
    ~HeapManager();

    static ObjectHeader *AllocateObject(size_t size);
    // TODO: implement AllocateFrame function
    static Frame *AllocateFrame();

private:
    size_t heap_size_ {0};
    void *heap_ {nullptr};
    // TODO: use free-list allocator to allocate object when this allocator will be implemented
    std::unique_ptr<AllocatorBase> objectAllocator_;
};

} // namespace evm::memory

#endif // MEMORY_HEAP_MANAGER_H
