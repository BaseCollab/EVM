#ifndef EVM_COMMON_OPCODE_TO_STR_H
#define EVM_COMMON_OPCODE_TO_STR_H

#include <string>

namespace evm {
enum Opcode : int;
} // namespace evm

namespace evm::common {

std::string OpcodeToString(Opcode opcode);

} // namespace evm::common

#endif // EVM_COMMON_OPCODE_TO_STR_H
