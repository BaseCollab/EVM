#ifndef EVM_RUNTIME_INTERPRETER_INL_H
#define EVM_RUNTIME_INTERPRETER_INL_H

#include "runtime/interpreter/interpreter.h"
#include "runtime/memory/types/array.h"
#include "runtime/memory/types/string.h"
#include "runtime/runtime.h"

namespace evm::runtime {

ALWAYS_INLINE int64_t HandleCreateArray(hword_t type, int32_t size)
{
    return reinterpret_cast<int64_t>(Array::Create(static_cast<memory::Type>(type), size));
}

ALWAYS_INLINE int64_t HandleLoadFromArray(int64_t array_ptr, int64_t idx)
{
    auto *array = reinterpret_cast<Array *>(array_ptr);
    int64_t value = 0;
    array->Get(&value, idx);
    return value;
}

ALWAYS_INLINE void HandleStoreToArray(int64_t array_ptr, int64_t array_idx, int64_t src_reg)
{
    auto *array = reinterpret_cast<Array *>(array_ptr);
    array->Set(src_reg, array_idx);
}

ALWAYS_INLINE int64_t HandleCreateStringObject(int32_t string_offset)
{
    auto *runtime = Runtime::GetInstance();

    const std::string *string = runtime->GetStringFromCache(string_offset);
    if (string == nullptr) {
        string = runtime->CreateStringAndSetInCache(string_offset);
    }

    return reinterpret_cast<int64_t>(String::Create(reinterpret_cast<const uint8_t *>(string->data()), string->size()));
}

ALWAYS_INLINE void HandlePrintString(int64_t string_ptr)
{
    auto *string = reinterpret_cast<String *>(string_ptr);
    printf("Print_str = %s\n", string->GetData());
}

ALWAYS_INLINE int64_t HandleStringConcatenation(int64_t lhs_string, int64_t rhs_string)
{
    return reinterpret_cast<int64_t>(
        String::ConcatStrings(reinterpret_cast<String *>(lhs_string), reinterpret_cast<String *>(rhs_string)));
}

ALWAYS_INLINE int64_t HandleStringComparison(int64_t lhs_string, int64_t rhs_string)
{
    return String::CompareStrings(reinterpret_cast<String *>(lhs_string), reinterpret_cast<String *>(rhs_string));
}

} // namespace evm::runtime

#endif // EVM_RUNTIME_INTERPRETER_INL_H
