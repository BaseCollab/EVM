#ifndef EVM_RUNTIME_MEMORY_MARK_WORD_H
#define EVM_RUNTIME_MEMORY_MARK_WORD_H

#include <cstdint>

namespace evm::runtime {

struct MarkWord {
    // 0 if object is not attended, 1 otherwise (used in all implemented GC)
    uint32_t mark : 1 = 0;
    // 0 if at least 1 neighbour of objects is not marked, 1 otherwise (used just in GC-incremental)
    uint32_t neighbour : 1 = 0;
    // For future purposes
    uint32_t reserved : 30 = 0;
    uint32_t hash = 0;
};

static_assert(sizeof(MarkWord) == 8);

} // namespace evm::runtime

#endif // EVM_RUNTIME_MEMORY_MARK_WORD_H
