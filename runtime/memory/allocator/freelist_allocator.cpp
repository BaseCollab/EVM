#include "runtime/memory/allocator/freelist_allocator.h"
#include <iostream>

namespace evm::runtime {

/* override */
void *FreelistAllocator::Alloc(size_t size)
{
    Node *prev_node = nullptr;
    Node *finded_memory_node = FindFirstFit(size, &prev_node);
    if (UNLIKELY(finded_memory_node == nullptr)) {
        // printf("[%s] No free memory in freelist allocator\n", __func__);
        UNREACHABLE();
    }

    size_t free_memory_in_block = finded_memory_node->block_size_;
    size_t required_memory = sizeof(AllocationHeader) + size;
    size_t remaining_memory = free_memory_in_block - required_memory;

    if (remaining_memory > 0 && remaining_memory >= sizeof(Node)) {
        uint8_t *remaining_memory_block = reinterpret_cast<uint8_t *>(finded_memory_node) + required_memory;
        Node *new_node = reinterpret_cast<Node *>(remaining_memory_block);
        InsertNode(finded_memory_node, new_node);
    }

    RemoveNode(prev_node, finded_memory_node);

    auto *header = reinterpret_cast<AllocationHeader *>(finded_memory_node);
    header->block_size = size;

    used_memory_size_ += required_memory;

    uint8_t *allocated_memory = reinterpret_cast<uint8_t *>(header) + sizeof(AllocationHeader);
    return reinterpret_cast<void *>(allocated_memory);
}

void FreelistAllocator::Dealloc(void *ptr_to_free)
{
    if (ptr_to_free == nullptr) {
        return;
    }

    uint8_t *header_raw = static_cast<uint8_t *>(ptr_to_free) - sizeof(AllocationHeader);
    auto *header = reinterpret_cast<AllocationHeader *>(header_raw);

    Node *node = reinterpret_cast<Node *>(header_raw);
    node->block_size_ = header->block_size;
    node->next_ = nullptr;

    if (last_freespace_node_ == nullptr) {
        Node *curr_node = head_node_;

        while (true) {
            if (curr_node->next_ == nullptr) {
                break;
            }
            curr_node = curr_node->next_;
        }
        curr_node->next_ = node;
        last_freespace_node_ = node;
    } else {
        last_freespace_node_->next_ = node;
    }

    used_memory_size_ -= (node->block_size_ + sizeof(AllocationHeader));
}

Node *FreelistAllocator::FindFirstFit(size_t size, Node **previous_node)
{
    Node *curr_node = head_node_;
    Node *prev_node = nullptr;

    while (true) {
        if (curr_node->block_size_ >= size) {
            break;
        }
        if (curr_node->next_ == nullptr) {
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
        head_node_ = node_to_remove->next_;
        return;
    }
    prev_node->next_ = node_to_remove->next_;
}

void FreelistAllocator::InsertNode(Node *prev_node, Node *new_node)
{
    assert(new_node != nullptr);

    if (prev_node == nullptr) {
        if (head_node_ == nullptr) {
            head_node_ = new_node;
            return;
        }

        head_node_->next_ = new_node;
        return;
    }

    if (prev_node->next_ == nullptr) {
        prev_node->next_ = new_node;
        new_node->next_ = nullptr;
        return;
    }

    new_node->next_ = prev_node->next_;
    prev_node->next_ = new_node;
}

} // namespace evm::runtime
