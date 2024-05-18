#ifndef EVM_RUNTIME_MEMORY_MARK_WORD_H
#define EVM_RUNTIME_MEMORY_MARK_WORD_H

#include <cstdint>

struct MarkWord {
    uint32_t mark : 1 = 0;
    uint32_t reserved : 31 = 0;
    uint32_t hash = 0;
};

static_assert(sizeof(MarkWord) == 8);

#endif // EVM_RUNTIME_MEMORY_MARK_WORD_H