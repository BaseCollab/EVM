#ifndef EVM_MEMORY_TYPES_ARRAY_INL_H
#define EVM_MEMORY_TYPES_ARRAY_INL_H

#include "array.h"

namespace evm::memory {

template <typename T>
void Array::Set(size_t idx, T elem)
{
    if (idx >= size_) {
        std::cerr << "Invalid idx = " << idx << "; size = " << size_ << std::endl;
        std::exit(EXIT_FAILURE);
    }
    auto *data = reinterpret_cast<T *>(data_);
    *(data + idx) = elem;
}

template <typename T>
T Array::Get(size_t idx) const
{
    auto *data = reinterpret_cast<T *>(data_);
    return *(data + idx);
}

} // namespace evm::memory

#endif // EVM_MEMORY_TYPES_ARRAY_INL_H
