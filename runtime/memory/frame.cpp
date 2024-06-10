#include "frame.h"

#include <cassert>
#include <bitset>
#include <array>
#include <cstddef>

namespace evm::runtime {

Frame::Frame(size_t restore_pc, const std::array<Register, N_PASSED_ARGS_DEFAULT> &passed_args)
    : restore_pc_ {restore_pc}
{
    for (size_t i = 0; i < passed_args.size(); ++i) {
        regs_[i] = passed_args[i];
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

void Frame::MarkReg(size_t reg_idx, bool is_root)
{
    assert(reg_idx < regs_.size());
    obj_regs_indicators_.set(reg_idx, is_root);
}

bool Frame::IsRegMarked(size_t reg_idx) const
{
    assert(reg_idx < regs_.size());
    return obj_regs_indicators_[reg_idx];
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
