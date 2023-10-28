#ifndef EVM_FRAME_H
#define EVM_FRAME_H

#include "common/macros.h"
#include "common/constants.h"
#include "reg.h"

#include <cassert>
#include <vector>
#include <cstddef>

namespace evm {

class Frame {
public:
    static constexpr size_t N_PASSED_ARGS_DEFAULT = 4; // only 4 passed args is supported now
    static constexpr size_t N_FRAME_REGS_DEFAULT = 1 << 8;
    static constexpr size_t N_FRAME_LOCAL_REGS_DEFAULT = N_FRAME_REGS_DEFAULT - N_PASSED_ARGS_DEFAULT;

public:
    Frame(size_t pc, size_t n_local_vars, 
        Register arg1 = Register(0.0), 
        Register arg2 = Register(0.0),
        Register arg3 = Register(0.0),
        Register arg4 = Register(0.0));

    ~Frame() = default;

    Register *GetReg(size_t reg_idx);
    size_t GetPC() const;
    void SetPC(size_t pc);

private:
    std::vector<Register> regs_;

    size_t pc_ {0};
};

} // namespace evm

#endif // EVM_FRAME_H
