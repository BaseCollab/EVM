#ifndef EVM_MEMORY_TYPE_H
#define EVM_MEMORY_TYPE_H

#include "common/logs.h"
#include "common/macros.h"

#include <string>
#include <cstdint>

namespace evm::memory {

// clang-format off
/// Negative numbers emphasize built-in types, positive ones — user-defined classes
enum class Type : int8_t {
    INVALID       = -1,
    DOUBLE        = -2,
    INT           = -3,
    CLASS_OBJECT  = -4,
    STRING_OBJECT = -5,
    ARRAY_OBJECT  = -6
};

enum TypeSize : uint8_t {
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
            PrintErr("Invalid type [", static_cast<int>(type), "]");
            return TypeSize::ZERO;
        case Type::DOUBLE:
            return TypeSize::DOUBLE;
        case Type::INT:
            return TypeSize::INT;
        case Type::CLASS_OBJECT:
            return TypeSize::OBJECT;
        case Type::STRING_OBJECT:
            return TypeSize::STRING;
        case Type::ARRAY_OBJECT:
            return TypeSize::ARRAY;
    }

    if (static_cast<int64_t>(type) >= 0) {
        return TypeSize::OBJECT;
    }

    PrintErr("Unsupported type [", static_cast<int>(type), "]");
    return TypeSize::ZERO;
}

static inline Type GetTypeFromString(std::string_view string)
{
    if (string == "double") {
        return Type::DOUBLE;
    } else if (string == "int") {
        return Type::INT;
    } else if (string == "obj") {
        return Type::CLASS_OBJECT;
    } else if (string == "class") {
        return Type::CLASS_OBJECT;
    } else if (string == "str") {
        return Type::STRING_OBJECT;
    } else if (string == "arr") {
        return Type::ARRAY_OBJECT;
    }
    return Type::INVALID;
}

static inline std::string GetStringFromType(Type type)
{
    switch (type) {
        case Type::INVALID:
            PrintErr("Invalid type [", static_cast<int>(type), "]");
            return std::string("<invalid>");
        case Type::DOUBLE:
            return std::string("double");
        case Type::INT:
            return std::string("int");
        case Type::CLASS_OBJECT:
            return std::string("obj");
        case Type::STRING_OBJECT:
            return std::string("str");
        case Type::ARRAY_OBJECT:
            return std::string("arr");
        default:
            PrintErr("Unsupported type [", static_cast<int>(type), "]");
            return std::string("<unsupported>");
    }
}

static inline bool IsReferenceType(Type type)
{
    switch (type) {
        case Type::DOUBLE:
        case Type::INT:
            return false;

        case Type::CLASS_OBJECT:
        case Type::STRING_OBJECT:
        case Type::ARRAY_OBJECT:
            return true;

        case Type::INVALID:
            PrintErr("Invalid type");
            UNREACHABLE();
    }

    if (static_cast<int64_t>(type) >= 0) {
        return true;
    }
    UNREACHABLE();
    return false;
}

static inline bool IsPrimitiveType(Type type)
{
    return !(IsReferenceType(type));
}

} // namespace evm::memory

#endif // EVM_MEMORY_TYPE_H
