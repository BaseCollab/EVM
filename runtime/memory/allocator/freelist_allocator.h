#ifndef EVM_RUNTIME_MEMORY_ALLOCATOR_FREELIST_ALLOCATOR_H
#define EVM_RUNTIME_MEMORY_ALLOCATOR_FREELIST_ALLOCATOR_H

#include "runtime/memory/allocator/allocator.h"

#include <cstdint>
#include <memory>

namespace evm::runtime {

struct AllocationHeader {
    size_t block_size {0};
};

struct Node {
    Node *next_ {nullptr};
    size_t block_size_ {0};
};

class FreelistAllocator final : public AllocatorBase {
public:
    NO_COPY_SEMANTIC(FreelistAllocator);
    NO_MOVE_SEMANTIC(FreelistAllocator);

    explicit FreelistAllocator(uint8_t *heap, size_t heap_capacity)
        : AllocatorBase(AllocatorBase::AllocatorType::FREELIST), heap_(heap), heap_capacity_(heap_capacity)
    {
        // place first node at the beginnig of the heap
        head_node_ = reinterpret_cast<Node *>(heap_);
        head_node_->next_ = nullptr;
        head_node_->block_size_ = heap_capacity_ - sizeof(AllocationHeader);

        tail_node_ = head_node_;
    }
    ~FreelistAllocator() override = default;

    void *Alloc(size_t size) override;
    void Dealloc(void *ptr) override;

    size_t GetHeapCapacity() const override
    {
        return heap_capacity_;
    }

    size_t GetUsedMemorySize() const
    {
        return used_memory_size_;
    }

private:
    Node *FindFirstFit(size_t size, Node **previous_node);

    void RemoveNode(Node *prev_node, Node *node_to_remove);

    void InsertNode(Node *prev_node, Node *new_node);

private:
    uint8_t *heap_ {nullptr};
    size_t heap_capacity_ {0};
    size_t used_memory_size_ {0};

    Node *head_node_ {nullptr};
    Node *tail_node_ {nullptr};
};

} // namespace evm::runtime

#endif // EVM_RUNTIME_MEMORY_ALLOCATOR_FREELIST_ALLOCATOR_H
