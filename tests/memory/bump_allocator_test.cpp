#include <gtest/gtest.h>

#include <array>
#include <cstring>
#include <sys/mman.h>

#include "runtime/memory/allocator/bump_allocator.h"
#include "common/constants.h"

namespace evm::runtime {

class BumpAllocatorTest : public testing::Test {
public:
    static constexpr size_t TEST_HEAP_SIZE = 32 * MBYTE_SIZE;

    void SetUp() override
    {
        heap_ = static_cast<uint8_t *>(
            mmap(nullptr, TEST_HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
        ASSERT_NE(heap_, nullptr);

        allocator_ = std::make_unique<BumpAllocator>(heap_, TEST_HEAP_SIZE);
    }

    void TearDown() override
    {
        int success = munmap(heap_, TEST_HEAP_SIZE);
        ASSERT_NE(success, -1);
    }

protected:
    uint8_t *heap_ {nullptr};
    std::unique_ptr<BumpAllocator> allocator_;
};

TEST_F(BumpAllocatorTest, GetBusySize)
{
    ASSERT_EQ(TEST_HEAP_SIZE, allocator_->GetHeapCapacity());
}

TEST_F(BumpAllocatorTest, Alloc)
{
    constexpr size_t arr_size = 1000000;

    std::array<uint8_t, MBYTE_SIZE> nmb;
    for (size_t i = 0; i < arr_size; ++i) {
        nmb[i] = i % 256;
    }

    auto *alloc_ptr = static_cast<uint8_t *>(allocator_->Alloc(nmb.size()));
    ASSERT_EQ(allocator_->GetBusySize(), nmb.size());

    std::memcpy(alloc_ptr, nmb.data(), nmb.size() * sizeof(uint8_t));

    for (size_t i = 0; i < arr_size; ++i) {
        ASSERT_EQ(*(alloc_ptr + i), i % 256);
    }
}

} // namespace evm::runtime
