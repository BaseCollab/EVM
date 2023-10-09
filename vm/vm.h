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

    freg_t GetFReg(freg_idx_t freg_idx) const
    {
        assert(freg_idx < N_FREG);
        return freg_table_[freg_idx];
    }

    void SetFReg(freg_idx_t freg_idx, freg_t value)
    {
        assert(freg_idx < N_FREG);
        freg_table_[freg_idx] = value;
    }

private:
    Interpreter interpreter_;

    reg_t reg_table_[N_REG] = {0};
    freg_t freg_table_[N_FREG] = {0};
};

} // namespace evm

#endif // EVM_VM_H
