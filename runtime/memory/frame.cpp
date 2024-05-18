#include "common/macros.h"
#include "common/constants.h"
#include "frame.h"

#include <cassert>
#include <bitset>
#include <vector>
#include <cstddef>
#include <iostream>

namespace evm::runtime {

Frame::Frame(size_t restore_pc, size_t n_local_vars, Register arg1, Register arg2, Register arg3, Register arg4)
    : restore_pc_ {restore_pc}
{
    regs_.push_back(arg1);
    regs_.push_back(arg2);
    regs_.push_back(arg3);
    regs_.push_back(arg4);

    for (size_t i = 0; i < n_local_vars; ++i) {
        regs_.push_back(Register(0.0));
    }

    obj_regs_indicators_.reset();
}

Register *Frame::GetReg(size_t reg_idx)
{
    assert(reg_idx < regs_.size());
    return &regs_[reg_idx];
}

const Register *Frame::GetReg(size_t reg_idx) const
{
    assert(reg_idx < regs_.size());
    return &regs_[reg_idx];
}

void Frame::MarkRegAsObject(size_t reg_idx)
{
    assert(reg_idx < regs_.size());
    obj_regs_indicators_.set(reg_idx, true);
}

void Frame::MarkRegAsNotObject(size_t reg_idx)
{
    assert(reg_idx < regs_.size());
    obj_regs_indicators_.set(reg_idx, false);
}

const std::bitset<Frame::N_FRAME_REGS_DEFAULT> &Frame::GetObjectBitMask() const
{
    return obj_regs_indicators_;
}

size_t Frame::GetRestorePC() const
{
    return restore_pc_;
}

void Frame::SetRestorePC(size_t restore_pc)
{
    restore_pc_ = restore_pc;
}

} // namespace evm::runtime
