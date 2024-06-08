#ifndef EVM_COMMON_UTILS_BITOPS_H
#define EVM_COMMON_UTILS_BITOPS_H

#include "common/constants.h"

#include <type_traits>
#include <cstring>

namespace evm::bitops {

template <typename T>
constexpr bit_size_t BitSizeof()
{
    return sizeof(T) * BITS_PER_BYTE;
}

constexpr dword_t Ones()
{
    return dword_t {0} - 1;
}

template <bit_size_t high, bit_size_t low>
constexpr dword_t GetBits(dword_t value)
{
    static_assert(high >= low);
    static_assert(high < BitSizeof<dword_t>());

    bit_size_t bit_size = high - low + 1;
    return (value >> low) & (Ones() >> (BitSizeof<dword_t>() - bit_size));
}

template <bit_size_t high, bit_size_t low>
constexpr dword_t Ones()
{
    static_assert(high >= low);
    return ~((Ones() >> (BitSizeof<dword_t>() - high - 1)) ^ (Ones() << low));
}

template <dword_t max, dword_t min>
constexpr dword_t Clamp(dword_t value)
{
    static_assert(max >= min);
    return value > max ? max : value < min ? min : value;
}

template <bit_size_t old_size, bit_size_t new_size>
constexpr dword_t SignExtend(dword_t value)
{
    static_assert(new_size > old_size);
    static_assert(new_size <= BitSizeof<dword_t>());
    static_assert(old_size > 0);

    bit_size_t shift = new_size - old_size;
    dword_t sign = GetBits<old_size - 1, old_size - 1>(value);
    dword_t mask = ((dword_t {1} << shift) - sign) << old_size;

    return GetBits<new_size - 1, 0>(mask | value);
}

template <typename T>
std::make_signed_t<T> MakeSigned(T unsigned_value)
{
    static_assert(std::is_unsigned_v<T>);
    return static_cast<std::make_signed_t<T>>(unsigned_value);
}

template <typename T>
std::make_unsigned_t<T> MakeUnsigned(T signed_value)
{
    static_assert(std::is_signed_v<T>);
    return static_cast<std::make_unsigned_t<T>>(signed_value);
}

template <typename To, typename From>
To BitCast(From src)
{
    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}

} // namespace evm::bitops

#endif // EVM_COMMON_UTILS_BITOPS_H
