#ifndef EVM_MEMORY_TYPE_H
#define EVM_MEMORY_TYPE_H

#include "common/macros.h"
#include "common/constants.h"

#include <iostream>
#include <string>

namespace evm::memory {

// clang-format off
enum class Type : int8_t {
    INVALID = -1,
    DOUBLE  = 0,
    INT     = 1,
    OBJECT  = 2,
    STRING  = 3,
    ARRAY   = 4
};

enum TypeSize : int8_t {
    ZERO    = 0,
    DOUBLE  = 8,
    INT     = 8,
    OBJECT  = 8,
    STRING  = 8,
    ARRAY   = 8
};
// clang-format on

static inline TypeSize GetSizeOfType(Type type)
{
    switch (type) {
        case Type::INVALID:
            std::cerr << __func__ << ": invalid array type [" << static_cast<int>(type) << "]" << std::endl;
            return TypeSize::ZERO;
        case Type::DOUBLE:
            return TypeSize::DOUBLE;
        case Type::INT:
            return TypeSize::INT;
        case Type::OBJECT:
            return TypeSize::OBJECT;
        case Type::STRING:
            return TypeSize::STRING;
        case Type::ARRAY:
            return TypeSize::ARRAY;
        default:
            std::cerr << __func__ << ": unsupported array type [" << static_cast<int>(type) << "]" << std::endl;
            return TypeSize::ZERO;
    }
}

static inline Type GetTypeFromString(std::string_view string)
{
    if (string == "double") {
        return Type::DOUBLE;
    } else if (string == "int") {
        return Type::INT;
    } else if (string == "obj") {
        return Type::OBJECT;
    } else if (string == "class") {
        return Type::OBJECT;
    } else if (string == "str") {
        return Type::STRING;
    } else if (string == "arr") {
        return Type::ARRAY;
    }
    return Type::INVALID;
}

static inline std::string GetStringFromType(Type type)
{
    switch (type) {
        case Type::INVALID:
            std::cerr << __func__ << ": invalid array type [" << static_cast<int>(type) << "]" << std::endl;
            return std::string("<invalid>");
        case Type::DOUBLE:
            return std::string("double");
        case Type::INT:
            return std::string("int");
        case Type::OBJECT:
            return std::string("obj");
        case Type::STRING:
            return std::string("str");
        case Type::ARRAY:
            return std::string("arr");
        default:
            std::cerr << __func__ << ": unsupported array type [" << static_cast<int>(type) << "]" << std::endl;
            return std::string("<unsupported>");
    }
}

} // namespace evm::memory

#endif // EVM_MEMORY_TYPE_H
