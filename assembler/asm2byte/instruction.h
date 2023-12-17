#ifndef EVM_ASSEMBLER_ASM_TO_BYTE__INSTURCTION_H
#define EVM_ASSEMBLER_ASM_TO_BYTE__INSTURCTION_H

#include "common/macros.h"
#include "common/constants.h"
#include "isa/opcodes.h"
#include "memory/frame.h"

#include <cassert>
#include <cstddef>
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>

namespace evm::asm2byte {

class Instruction {
public:
    static constexpr size_t MINIMAL_INSTR_SIZE = 4;

public:
    struct Immediate {
    public:
        Immediate() = default;
        ~Immediate() = default;

        explicit Immediate(int64_t imm) : imm64_(imm)
        {
            is_set_ = true;
            num_bytes_ = sizeof(int64_t);
        }

        explicit Immediate(int32_t imm) : imm32_(imm)
        {
            is_set_ = true;
            num_bytes_ = sizeof(int32_t);
        }

    public:
        bool is_set_ {false};

        int64_t imm64_ {0};
        int32_t imm32_ {0};
        size_t num_bytes_ {0};
    };

public:
    DEFAULT_MOVE_SEMANTIC(Instruction);
    DEFAULT_COPY_SEMANTIC(Instruction);

    Instruction(Opcode opcode) : opcode_(opcode) {}
    ~Instruction() = default;

    void SetRd(byte_t rd)
    {
        rd_ = rd;
    }

    void SetRs1(byte_t rs1)
    {
        rs1_ = rs1;
    }

    void SetRs2(byte_t rs2)
    {
        rs2_ = rs2;
    }

    void SetRs3(byte_t rs3)
    {
        rd_ = rs3;
    }

    void Set32Imm(int32_t imm)
    {
        imm_ = Immediate(imm);
    }

    void Set64Imm(int64_t imm)
    {
        imm_ = Immediate(imm);
    }

    void Add32Imm(int32_t imm_add)
    {
        imm_ = Immediate(imm_.imm32_ + imm_add);
    }

    void Add64Imm(int64_t imm_add)
    {
        imm_ = Immediate(imm_.imm64_ + imm_add);
    }

    size_t GetImmSize() const
    {
        return imm_.num_bytes_;
    }

    void SetArg(size_t arg_num, byte_t arg_reg)
    {
        have_args_ = true;

        assert(arg_num < Frame::N_PASSED_ARGS_DEFAULT);
        args_[arg_num] = arg_reg;
    }

    void SetOffset(size_t offset)
    {
        bytcode_offset_ = offset;
    }

    void InstrToBytes(std::vector<byte_t> *out_arr)
    {
        out_arr->push_back(static_cast<byte_t>(opcode_));
        out_arr->push_back(rd_);
        out_arr->push_back(rs1_);
        out_arr->push_back(rs2_);

        if (imm_.is_set_) {
            size_t prev_size = out_arr->size();
            out_arr->insert(out_arr->end(), imm_.num_bytes_, 0);
            void *imm_ptr =
                (imm_.num_bytes_ == 4) ? static_cast<void *>(&(imm_.imm32_)) : static_cast<void *>(&(imm_.imm64_));
            std::memcpy(out_arr->data() + prev_size, imm_ptr, imm_.num_bytes_);
        } else if (have_args_ == true) {
            for (size_t i = 0; i < Frame::N_PASSED_ARGS_DEFAULT; ++i) {
                out_arr->push_back(args_[i]);
            }
        }
    }

    size_t GetBytesSize() const
    {
        return MINIMAL_INSTR_SIZE + imm_.num_bytes_ + (have_args_ == true) * Frame::N_PASSED_ARGS_DEFAULT;
    }

private:
    size_t bytcode_offset_;

    Opcode opcode_ {Opcode::INVALID};

    byte_t rd_ {0};
    byte_t rs1_ {0};
    byte_t rs2_ {0};

    Immediate imm_;

    bool have_args_ {false};
    byte_t args_[Frame::N_PASSED_ARGS_DEFAULT] = {0};
};

} // namespace evm::asm2byte

#endif // EVM_ASSEMBLER_ASM_TO_BYTE__INSTURCTION_H
