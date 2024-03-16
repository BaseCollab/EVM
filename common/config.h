#ifndef EVM_COMMON_CONFIG_H
#define EVM_COMMON_CONFIG_H

#include "constants.h"

#include <cstddef>
#include <cstdint>

namespace evm {

// clang-format off

using freg_t = double;
using reg_t  = dword_t;
using addr_t = reg_t;

namespace config {
    size_t DEFAULT_HEAP_SIZE = 32 * MBYTE_SIZE;
} // namespace evm::config

// clang-format on

} // namespace evm

#endif // EVM_COMMON_CONFIG_H
