#ifndef EVM_MEMORY_ALLOCATOR_H
#define EVM_MEMORY_ALLOCATOR_H

#include "common/macros.h"

#include <cstddef>
#include <memory>

namespace evm::memory {

class AllocatorBase {
public:
    enum class AllocatorType {
        INVALID = -1,
        BUMP = 0,
    };

    static constexpr size_t DEFAULT_HEAP_SIZE = 1 << 20; // 1 Mb

public:
    NO_COPY_SEMANTIC(AllocatorBase);
    NO_MOVE_SEMANTIC(AllocatorBase);

    explicit AllocatorBase() = default;
    virtual ~AllocatorBase() = default;

    virtual AllocatorType GetAllocatorType() const = 0;

    virtual void *Alloc(size_t size) = 0;

    virtual size_t GetHeapCapacity() const = 0;
};

} // namespace evm::memory

#endif // EVM_MEMORY_ALLOCATOR_H
