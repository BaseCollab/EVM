#ifndef EVM_MEMORY_FRAME_H
#define EVM_MEMORY_FRAME_H

#include "common/macros.h"
#include "common/constants.h"
#include "runtime/memory/reg.h"

#include <array>
#include <cstddef>
#include <bitset>

namespace evm::runtime {

class Frame {
public:
    static constexpr size_t N_PASSED_ARGS_DEFAULT = 4; // only 4 passed args is supported now
    static constexpr size_t N_FRAME_REGS_DEFAULT = 1 << 8;
    static constexpr size_t N_FRAME_LOCAL_REGS_DEFAULT = N_FRAME_REGS_DEFAULT - N_PASSED_ARGS_DEFAULT;

public:
    Frame(size_t restore_pc, const std::array<Register, N_PASSED_ARGS_DEFAULT> &passed_args);

    ~Frame() = default;

    Register *GetReg(size_t reg_idx);
    const Register *GetReg(size_t reg_idx) const;

    size_t GetRestorePC() const;
    void SetRestorePC(size_t pc);

    bool IsRegMarked(size_t reg_idx) const;
    void MarkReg(size_t reg_idx, bool is_root = true);

    const std::bitset<N_FRAME_REGS_DEFAULT> &GetObjectBitMask() const;

private:
    std::array<Register, N_FRAME_REGS_DEFAULT> regs_;
    std::bitset<N_FRAME_REGS_DEFAULT> obj_regs_indicators_;

    size_t restore_pc_ {0}; // pc to save before call of another function
};

} // namespace evm::runtime

#endif // EVM_MEMORY_FRAME_H
