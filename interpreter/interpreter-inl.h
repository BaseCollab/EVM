#ifndef EVM_INTERPRETER_INL_H
#define EVM_INTERPRETER_INL_H

#include "interpreter/interpreter.h"
#include "memory/types/array.h"

namespace evm {

ALWAYS_INLINE HandleLoadFromArray(int64_t array_ptr, int64_t idx)
{
    memory::Array *array = reinterpret_cast<memory::Array *>(array_ptr);
    int64_t value = 0;      
    array->Get(&value, idx);
    return value;
}

ALWAYS_INLINE HandleStoreToArray(int64_t array_ptr, int64_t array_idx, int64_t src_reg)
{
    memory::Array *array = reinterpret_cast<memory::Array *>(array_ptr);
    array->Set(src_reg, array_idx);
}

} // namespace evm

#endif // EVM_INTERPRETER_INL_H
