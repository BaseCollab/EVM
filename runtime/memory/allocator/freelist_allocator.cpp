#include "common/logs.h"
#include "runtime/memory/allocator/freelist_allocator.h"

namespace evm::runtime {

/* override */
void *FreelistAllocator::Alloc(size_t size)
{
    if (head_node_ == nullptr) {
        return nullptr;
    }

    Node *prev_node = nullptr;
    Node *finded_memory_node = FindFirstFit(size, &prev_node);

    size_t free_memory_in_block = finded_memory_node->block_size_;
    int64_t remaining_memory = free_memory_in_block - size;

    if (remaining_memory > 0 && static_cast<uint64_t>(remaining_memory) >= sizeof(Node)) {
        uint8_t *remaining_memory_block =
            reinterpret_cast<uint8_t *>(finded_memory_node) + size + sizeof(AllocationHeader);

        Node *new_node = reinterpret_cast<Node *>(remaining_memory_block);
        new_node->block_size_ = remaining_memory - sizeof(AllocationHeader);

        InsertNode(finded_memory_node, new_node);
    }

    RemoveNode(prev_node, finded_memory_node);

    assert(finded_memory_node != nullptr);

    auto *header = reinterpret_cast<AllocationHeader *>(finded_memory_node);
    header->block_size = size;

    used_memory_size_ += size + sizeof(AllocationHeader);

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
    node->block_size_ = header->block_size;
    node->next_ = nullptr;

    if (tail_node_ == nullptr) {
        // It means that all memory was allocated
        head_node_ = node;
        tail_node_ = head_node_;
    } else {
        tail_node_->next_ = node;
        tail_node_ = tail_node_->next_;
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
        if (node_to_remove->next_ == nullptr) {
            tail_node_ = nullptr;
        }
    } else {
        prev_node->next_ = node_to_remove->next_;
        if (node_to_remove->next_ == nullptr) {
            tail_node_ = prev_node;
        }
    }
}

void FreelistAllocator::InsertNode(Node *prev_node, Node *new_node)
{
    assert(new_node != nullptr);

    if (prev_node == nullptr) {
        if (head_node_ == nullptr) {
            head_node_ = new_node;
            tail_node_ = head_node_;
            return;
        }

        if (head_node_->next_ == nullptr) {
            tail_node_ = new_node;
        }
        head_node_->next_ = new_node;
        return;
    }

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
