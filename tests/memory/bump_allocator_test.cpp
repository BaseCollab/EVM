#include <gtest/gtest.h>

#include <array>
#include <cstring>

#include "runtime/memory/allocator/bump_allocator.h"

namespace evm::memory {

class BumpAllocatorTest : public testing::Test {
public:
    static constexpr size_t TEST_HEAP_SIZE = 1000;

    void SetUp() override
    {
        allocator_ = std::make_unique<BumpAllocator>(TEST_HEAP_SIZE);
    }

    void TearDown() override {}

protected:
    std::unique_ptr<BumpAllocator> allocator_;
};

TEST_F(BumpAllocatorTest, GetBusySize)
{
    ASSERT_EQ(TEST_HEAP_SIZE, allocator_->GetHeapCapacity());
}

TEST_F(BumpAllocatorTest, Alloc)
{
    std::array<uint8_t, TEST_HEAP_SIZE> nmb;

    for (size_t i = 0; i < TEST_HEAP_SIZE; ++i) {
        nmb[i] = i % 256;
    }

    auto *alloc_ptr = static_cast<uint8_t *>(allocator_->Alloc(nmb.size()));
    ASSERT_EQ(allocator_->GetBusySize(), nmb.size());

    std::memcpy(alloc_ptr, nmb.data(), nmb.size() * sizeof(uint8_t));

    for (size_t i = 0; i < TEST_HEAP_SIZE; ++i) {
        ASSERT_EQ(*(alloc_ptr + i), i % 256);
    }
}

} // namespace evm::memory
