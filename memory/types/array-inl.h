#ifndef MEMORY_TYPES_ARRAY_INL_H
#define MEMORY_TYPES_ARRAY_INL_H

#include "array.h"

namespace evm::memory {

template<typename T>
Array::Type GetArrayTypeFromPrimitive()
{
    if constexpr (std::is_same<T, double>::value) {
        return Type::DOUBLE;
    }
    if constexpr (std::is_same<T, int64_t>::value) {
        return Type::INT;
    }
    if constexpr (std::is_same<T, uint8_t>::value) {
        return Type::BYTE;
    }
    return Type::INVALID;
}

template<typename T>
void Array::SetPrimitive(T value, size_t idx)
{
    auto type = GetArrayTypeFromPrimitive<T>();
    if (type == Type::INVALID) {
        std::cerr << "Passed invalid type into SetPrimitive" << std::endl;
        return;
    }

    std::memcpy(data_ + idx * sizeof(T), &value, sizeof(T));
}

template<typename T>
void Array::GetPrimitive(T *value, size_t idx) const
{
    auto type = GetArrayTypeFromPrimitive<T>();
    if (type == Type::INVALID) {
        std::cerr << "Passed invalid type into GetPrimitive" << std::endl;
        return;
    }

    std::memcpy(value, data_ + idx * sizeof(T), sizeof(T));
}

} // namespace evm::memory

#endif // MEMORY_TYPES_ARRAY_INL_H
