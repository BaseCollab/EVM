#ifndef EVM_COMMON_STR_TO_OPCODE_H
#define EVM_COMMON_STR_TO_OPCODE_H

#include <string>

namespace evm {
enum Opcode : int;
} // namespace evm

namespace evm::common {

Opcode StringToOpcode(const std::string &opcode_str);

} // namespace evm::common

#endif // EVM_COMMON_STR_TO_OPCODE_H
