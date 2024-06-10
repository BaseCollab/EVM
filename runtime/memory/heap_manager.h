#ifndef RUNTIME_MEMORY_HEAP_MANAGER_H
#define RUNTIME_MEMORY_HEAP_MANAGER_H

#include "runtime/memory/allocator/allocator.h"
#include "runtime/memory/object_header.h"

#include <vector>
#include <cstddef>
#include <memory>

namespace evm::runtime {
class Frame;
} // namespace evm::runtime

namespace evm::runtime {

class HeapManager {
public:
    NO_COPY_SEMANTIC(HeapManager);
    NO_MOVE_SEMANTIC(HeapManager);

    explicit HeapManager(size_t heap_size);
    ~HeapManager();

    const std::vector<ObjectHeader *> &GetObjectsList() const;

    void *AllocateObject(size_t size);
    void *AllocateInternalObject(size_t size);

    // TODO: implement AllocateFrame function
    Frame *AllocateFrame();

private:
    size_t heap_size_ {0};
    uint8_t *heap_ {nullptr};
    // TODO: use free-list allocator object allocations when this allocator will be implemented.
    std::unique_ptr<AllocatorBase> object_allocator_;
    // TODO: create allocator for internal allocations.

    std::vector<ObjectHeader *> objects_;
};

} // namespace evm::runtime

#endif // RUNTIME_MEMORY_HEAP_MANAGER_H
