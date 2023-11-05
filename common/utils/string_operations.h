#ifndef EVM_COMMON_UTILS_STRING_OPERATIONS_H
#define EVM_COMMON_UTILS_STRING_OPERATIONS_H

#include <string>
#include <sstream>

namespace evm::common {

std::string StrToLower(std::string str);

template <typename Numeric>
bool IsNumber(const std::string &str)
{
    Numeric number;
    return ((std::istringstream(str) >> number >> std::ws).eof());
}

} // namespace evm::common

#endif // EVM_COMMON_UTILS_STRING_OPERATIONS_H
