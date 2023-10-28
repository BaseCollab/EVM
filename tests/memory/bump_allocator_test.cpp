#include <gtest/gtest.h>

#include "memory/allocator/bump_allocator.h"

namespace evm::memory {

class BumpAllocatorTest : public testing::Test {
public:
    static constexpr size_t TEST_HEAP_SIZE = 1000;

    void SetUp() override
    {
        std::make_unique<BumpAllocator>(TEST_HEAP_SIZE);
    }

    void TearDown() override {}

protected:
    std::unique_ptr<BumpAllocator> allocator_;
};

TEST_F(BumpAllocatorTest, GetBusySize)
{
    ASSERT_EQ(TEST_HEAP_SIZE, allocator_->GetBusySize());
}

} // namespace evm::memory
