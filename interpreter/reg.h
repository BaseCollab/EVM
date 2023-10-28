#ifndef EVM_REG_REG_H
#define EVM_REG_REG_H

#include "common/macros.h"
#include "common/constants.h"

#include <cstddef>

namespace evm {

class Register {
public:
    Register() = default;
    ~Register() = default;

    double GetDouble() const
    {
        return value_d;
    }

    int64_t GetInt64() const
    {
        return value_i;
    }

    byte_t *GetPtr() const
    {
        return value_ptr;
    }

    void SetDouble(double value)
    {
        value_d = value;
    }

    void SetInt64(int64_t value)
    {
        value_i = value;
    }

    void SetPtr(byte_t *ptr)
    {
        value_ptr = ptr;
    }

private:
    union {
        double value_d;
        int64_t value_i;
        byte_t *value_ptr;
    };
};

} // namespace evm

#endif // EVM_REG_REG_H
