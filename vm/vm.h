#ifndef EVM_VM_H
#define EVM_VM_H

#include "common/constants.h"
#include "common/config.h"
#include "common/macros.h"
#include "interpreter/interpreter.h"
#include "isa/regs.h"

#include "cassert"

namespace evm {

class VirtualMachine {
public:
    NO_COPY_SEMANTIC(VirtualMachine);
    NO_MOVE_SEMANTIC(VirtualMachine);

    VirtualMachine() = default;
    ~VirtualMachine() = default;

    void Execute(const byte_t *bytecode);

    reg_t GetReg(reg_idx_t reg_idx) const
    {
        assert(reg_idx < N_REG);
        return reg_table_[reg_idx];
    }

    void SetReg(reg_idx_t reg_idx, reg_t value)
    {
        assert(reg_idx < N_REG);
        reg_table_[reg_idx] = value;
    }

private:
    Interpreter interpreter_;

    reg_t reg_table_[N_REG] = {0};
};

} // namespace evm

#endif // EVM_VM_H
