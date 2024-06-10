#include <gtest/gtest.h>

#include "runtime/memory/allocator/freelist_allocator.h"
#include "common/constants.h"

#include <sys/mman.h>

namespace evm::runtime {

class FreeListAllocatorTest : public testing::Test {
public:
    static constexpr size_t TEST_HEAP_SIZE = 32 * MBYTE_SIZE;

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
    constexpr size_t TEST_SIZE = 1000;

    void *allocated_ptr[TEST_SIZE] = {};
    allocated_ptr[0] = allocator_->Alloc(TEST_SIZE);
    uint8_t *first_object = reinterpret_cast<uint8_t *>(allocated_ptr[0]);

    for (size_t idx = 1; idx < TEST_SIZE; ++idx) {
        allocated_ptr[idx] = allocator_->Alloc(TEST_SIZE);
        ASSERT_EQ(reinterpret_cast<uint8_t *>(allocated_ptr[idx]),
                  first_object + idx * (TEST_SIZE + sizeof(AllocationHeader)));
    }

    ASSERT_EQ(allocator_->GetUsedMemorySize(), TEST_SIZE * (TEST_SIZE + sizeof(AllocationHeader)));
}

TEST_F(FreeListAllocatorTest, Deallocate)
{
    constexpr size_t TEST_SIZE = 1000;

    void *allocated_ptr[TEST_SIZE] = {};
    allocated_ptr[0] = allocator_->Alloc(TEST_SIZE);
    uint8_t *first_object = reinterpret_cast<uint8_t *>(allocated_ptr[0]);

    for (size_t idx = 1; idx < TEST_SIZE; ++idx) {
        allocated_ptr[idx] = allocator_->Alloc(TEST_SIZE);
    }

    size_t before_dealloc_used_memory = allocator_->GetUsedMemorySize();

    uint8_t *deallocate_pos = first_object + 10 * (TEST_SIZE + sizeof(AllocationHeader));
    allocator_->Dealloc(deallocate_pos);

    size_t after_dealloc_used_memory = allocator_->GetUsedMemorySize();
    ASSERT_EQ(after_dealloc_used_memory, before_dealloc_used_memory - TEST_SIZE - sizeof(AllocationHeader));

    uint8_t *new_allocation = static_cast<uint8_t *>(allocator_->Alloc(TEST_SIZE));

    ASSERT_EQ(deallocate_pos, new_allocation);
}

} // namespace evm::runtime
