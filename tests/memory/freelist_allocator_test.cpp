#include <gtest/gtest.h>

#include "runtime/memory/allocator/freelist_allocator.h"
#include "common/constants.h"

#include <sys/mman.h>

namespace evm::runtime {

class FreeListAllocatorTest : public testing::Test {
public:
    static constexpr size_t ALLOCATION_HEADER_SIZE = sizeof(AllocationHeader);
    static constexpr size_t TEST_OBJECT_SIZE = 1000;
    static constexpr size_t NUMBER_OF_TEST_OBJECTS = 1000;
    static constexpr size_t FULL_TEST_OBJECT_SIZE = TEST_OBJECT_SIZE + ALLOCATION_HEADER_SIZE;
    static constexpr size_t TEST_HEAP_SIZE = NUMBER_OF_TEST_OBJECTS * FULL_TEST_OBJECT_SIZE;

    void SetUp() override
    {
        heap_ = static_cast<uint8_t *>(
            mmap(nullptr, TEST_HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
        ASSERT_NE(heap_, nullptr);

        allocator_ = std::make_unique<FreelistAllocator>(heap_, TEST_HEAP_SIZE);
    }

    void TearDown() override
    {
        int success = munmap(heap_, TEST_HEAP_SIZE);
        ASSERT_NE(success, -1);
    }

protected:
    uint8_t *heap_ {nullptr};
    std::unique_ptr<FreelistAllocator> allocator_;
};

TEST_F(FreeListAllocatorTest, Allocate)
{
    void *allocated_ptr[NUMBER_OF_TEST_OBJECTS] = {};
    allocated_ptr[0] = allocator_->Alloc(TEST_OBJECT_SIZE);
    ASSERT_NE(allocated_ptr[0], nullptr);

    uint8_t *first_object = reinterpret_cast<uint8_t *>(allocated_ptr[0]);

    for (size_t idx = 1; idx < NUMBER_OF_TEST_OBJECTS; ++idx) {
        allocated_ptr[idx] = allocator_->Alloc(TEST_OBJECT_SIZE);

        ASSERT_NE(allocated_ptr[idx], nullptr);

        ASSERT_EQ(reinterpret_cast<uint8_t *>(allocated_ptr[idx]),
                  first_object + idx * (TEST_OBJECT_SIZE + sizeof(AllocationHeader)));
    }

    ASSERT_EQ(allocator_->Alloc(TEST_OBJECT_SIZE), nullptr);
    ASSERT_EQ(allocator_->GetUsedMemorySize(), NUMBER_OF_TEST_OBJECTS * FULL_TEST_OBJECT_SIZE);
}

TEST_F(FreeListAllocatorTest, Deallocate)
{
    void *allocated_ptr[NUMBER_OF_TEST_OBJECTS] = {};
    allocated_ptr[0] = allocator_->Alloc(TEST_OBJECT_SIZE);
    uint8_t *first_object = reinterpret_cast<uint8_t *>(allocated_ptr[0]);

    for (size_t idx = 1; idx < NUMBER_OF_TEST_OBJECTS; ++idx) {
        allocated_ptr[idx] = allocator_->Alloc(TEST_OBJECT_SIZE);
    }

    size_t before_dealloc_used_memory = allocator_->GetUsedMemorySize();

    uint8_t *deallocate_pos = first_object + 10 * FULL_TEST_OBJECT_SIZE;
    allocator_->Dealloc(deallocate_pos);

    size_t after_dealloc_used_memory = allocator_->GetUsedMemorySize();
    ASSERT_EQ(after_dealloc_used_memory, before_dealloc_used_memory - FULL_TEST_OBJECT_SIZE);

    uint8_t *new_allocation = static_cast<uint8_t *>(allocator_->Alloc(TEST_OBJECT_SIZE));

    ASSERT_EQ(deallocate_pos, new_allocation);
}

TEST_F(FreeListAllocatorTest, DeallocateManyObjects)
{
    void *allocated_ptr[NUMBER_OF_TEST_OBJECTS] = {};
    allocated_ptr[0] = allocator_->Alloc(TEST_OBJECT_SIZE);
    uint8_t *first_object = reinterpret_cast<uint8_t *>(allocated_ptr[0]);

    for (size_t idx = 1; idx < NUMBER_OF_TEST_OBJECTS; ++idx) {
        allocated_ptr[idx] = allocator_->Alloc(TEST_OBJECT_SIZE);
    }

    constexpr size_t DEALLOC_POSITIONS_SIZE = 8;
    size_t dealloc_positions[DEALLOC_POSITIONS_SIZE] = {10, 11, 12, 200, 201, 300, 400, 500};
    uint8_t *dealloc_pointers[DEALLOC_POSITIONS_SIZE] = {};

    for (size_t idx = 0; idx < DEALLOC_POSITIONS_SIZE; ++idx) {
        dealloc_pointers[idx] = first_object + dealloc_positions[idx] * FULL_TEST_OBJECT_SIZE;
        allocator_->Dealloc(dealloc_pointers[idx]);
    }

    for (size_t idx = 0; idx < DEALLOC_POSITIONS_SIZE; ++idx) {
        uint8_t *new_allocation = static_cast<uint8_t *>(allocator_->Alloc(TEST_OBJECT_SIZE));
        uint8_t *deallocate_pos = dealloc_pointers[idx];

        ASSERT_EQ(deallocate_pos, new_allocation);
    }
}

} // namespace evm::runtime
