#ifndef EVM_COMMON_CONFIG_H
#define EVM_COMMON_CONFIG_H

#include "constants.h"
#include "utils/bitops.h"

#include <cstddef>

namespace evm {

// clang-format off

using freg_t = double;
using reg_t  = dword_t;
using addr_t = reg_t;

static constexpr size_t MAX_SUPPORTED_CLASSES_LOG2 = bitops::BitSizeof<hword_t>() - 1;
static constexpr size_t MAX_SUPPORTED_CLASSES      = 1 << MAX_SUPPORTED_CLASSES_LOG2;

// clang-format on

} // namespace evm

#endif // EVM_COMMON_CONFIG_H
