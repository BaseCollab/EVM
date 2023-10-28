#ifndef EVM_FRAME_H
#define EVM_FRAME_H

#include "common/macros.h"
#include "common/constants.h"
#include "reg.h"

#include <cstddef>

namespace evm {

class Frame {
public:
    static constexpr size_t PASSED_ARGUMENTS = 4; // only 4 passed regs is supported now

public:
    NO_COPY_SEMANTIC(Frame);
    NO_MOVE_SEMANTIC(Frame);

    Frame() = default;
    ~Frame() = default;

    Register *GetReg(size_t reg_idx) const
    {
        assert(reg_idx < regs_.size());
        return &regs_[reg_idx];
    }

private:
    std::vector<Register> regs_;

    size_t pc_ {0};
};

} // namespace evm

#endif // EVM_FRAME_H
