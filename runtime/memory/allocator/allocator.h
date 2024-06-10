#ifndef EVM_RUNTIME_MEMORY_ALLOCATOR_ALLOCATOR_H
#define EVM_RUNTIME_MEMORY_ALLOCATOR_ALLOCATOR_H

#include "common/macros.h"
#include <cstddef>

namespace evm::runtime {

class Array;

class AllocatorBase {
public:
    enum class AllocatorType {
        INVALID = -1,
        BUMP = 0,
        FREELIST = 1,
    };

public:
    NO_COPY_SEMANTIC(AllocatorBase);
    NO_MOVE_SEMANTIC(AllocatorBase);

    explicit AllocatorBase(AllocatorType allocator_type) : allocator_type_(allocator_type) {};
    virtual ~AllocatorBase() = default;

    AllocatorType GetAllocatorType() const
    {
        return allocator_type_;
    }

    virtual void *Alloc(size_t size) = 0;

    virtual size_t GetHeapCapacity() const = 0;

private:
    AllocatorType allocator_type_ {AllocatorType::INVALID};
};

} // namespace evm::runtime

#endif // EVM_RUNTIME_MEMORY_ALLOCATOR_ALLOCATOR_H
