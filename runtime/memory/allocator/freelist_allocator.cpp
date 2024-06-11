#include "common/logs.h"
#include "runtime/memory/allocator/freelist_allocator.h"
#include <cstdio>

namespace evm::runtime {

/* override */
void *FreelistAllocator::Alloc(size_t size)
{
    if (head_node_ == nullptr) {
        return nullptr; // no free space to allocate
    }

    Node *prev_node = nullptr;
    Node *found_memory_node = FindFirstFit(size, &prev_node);
    if (found_memory_node == nullptr) {
        PrintErr("Allocation failed: no fitting blocks for size ", size);
        return nullptr;
    }

    uint64_t remainder_block_size = found_memory_node->block_size_ - size;

    // Create new node in free blocks list
    if (remainder_block_size >= sizeof(Node)) {
        uint8_t *remainder_block_ptr = reinterpret_cast<uint8_t *>(found_memory_node) + size + sizeof(AllocationHeader);
        Node *remainder_block_node = reinterpret_cast<Node *>(remainder_block_ptr);

        remainder_block_node->block_size_ = remainder_block_size - sizeof(AllocationHeader);

        InsertNode(found_memory_node, remainder_block_node);
    }

    RemoveNode(prev_node, found_memory_node);

    auto *header = reinterpret_cast<AllocationHeader *>(found_memory_node);
    header->block_size = size + (remainder_block_size < sizeof(Node)) * remainder_block_size;

    used_memory_size_ += header->block_size + sizeof(AllocationHeader);
    uint8_t *allocated_memory = reinterpret_cast<uint8_t *>(header) + sizeof(AllocationHeader);

    return reinterpret_cast<void *>(allocated_memory);
}

void FreelistAllocator::Dealloc(void *ptr)
{
    if (ptr == nullptr) {
        return;
    }

    uint8_t *header_raw = static_cast<uint8_t *>(ptr) - sizeof(AllocationHeader);

    auto *header = reinterpret_cast<AllocationHeader *>(header_raw);
    Node *node = reinterpret_cast<Node *>(header_raw);

    size_t block_size = header->block_size;
    // Created node for dealloc space

    node->block_size_ = block_size;
    node->next_ = nullptr;

    PrintLog("Dealloc ", block_size, " ptr ", (long)ptr);

    if (tail_node_ == nullptr) {
        // It means that all memory was allocated
        head_node_ = node;
        tail_node_ = head_node_;
    } else {
        tail_node_->next_ = node;
        tail_node_ = node;
    }

    used_memory_size_ -= (node->block_size_ + sizeof(AllocationHeader));
}

Node *FreelistAllocator::FindFirstFit(size_t size, Node **previous_node)
{
    assert(head_node_ != nullptr);

    Node *curr_node = head_node_;
    Node *prev_node = nullptr;

    while (true) {
        if (curr_node->block_size_ >= size) {
            break;
        }
        if (curr_node->next_ == nullptr) {
            prev_node = curr_node;
            curr_node = nullptr;
            break;
        }
        prev_node = curr_node;
        curr_node = curr_node->next_;
    }

    if (prev_node != nullptr) {
        *previous_node = prev_node;
    }

    return curr_node;
}

void FreelistAllocator::RemoveNode(Node *prev_node, Node *node_to_remove)
{
    assert(node_to_remove != nullptr);

    if (prev_node == nullptr) {
        // (node_to_remove == head_node_) == 1
        head_node_ = node_to_remove->next_;
        if (node_to_remove->next_ == nullptr) {
            tail_node_ = head_node_;
        }
    } else {
        prev_node->next_ = node_to_remove->next_;
        if (prev_node->next_ == nullptr) {
            tail_node_ = prev_node;
        }
    }
}

void FreelistAllocator::InsertNode(Node *prev_node, Node *new_node)
{
    assert(new_node != nullptr);

    if (prev_node->next_ == nullptr) {
        prev_node->next_ = new_node;
        new_node->next_ = nullptr;
        tail_node_ = new_node;
        return;
    }

    new_node->next_ = prev_node->next_;
    prev_node->next_ = new_node;
}

} // namespace evm::runtime
