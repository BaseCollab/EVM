#ifndef EVM_COMMON_CONSTANTS_H
#define EVM_COMMON_CONSTANTS_H

#include <cstddef>
#include <cstdint>

namespace evm {

// clang-format off

using byte_t = uint8_t;
static constexpr size_t BYTE_SIZE = sizeof(byte_t);

using hword_t = uint16_t;
static constexpr size_t HWORD_SIZE = sizeof(hword_t);

using word_t = uint32_t;
static constexpr size_t WORD_SIZE = sizeof(word_t);

using dword_t = uint64_t;
static constexpr size_t DWORD_SIZE = sizeof(dword_t);

static constexpr size_t BITS_PER_BYTE = 1 << 3;
static constexpr size_t KBYTE_SIZE    = 1 << 10;
static constexpr size_t MBYTE_SIZE    = 1 << 20;

// clang-format on

} // namespace evm

#endif // EVM_COMMON_CONSTANTS_H