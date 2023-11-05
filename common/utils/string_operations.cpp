#include "string_operations.h"

#include <algorithm>

namespace evm::common {

std::string StrToLower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
    return str;
}

} // namespace evm::common
