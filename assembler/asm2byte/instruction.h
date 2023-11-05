#ifndef EVM_ASSEMBLER_ASM_TO_BYTE__INSTURCTION_H
#define EVM_ASSEMBLER_ASM_TO_BYTE__INSTURCTION_H

#include "common/macros.h"
#include "common/constants.h"

#include "isa/opcodes.h"

#include <cstddef>

#include <fstream>

#include <cstring>

#include <vector>
#include <iostream>

namespace evm::asm2byte {

class Instruction {
public:
    struct Immediate {
    public:
        Immediate() = default;
        ~Immediate() = default;

        explicit Immediate(int64_t imm) : imm64_(imm)
        {
            is_set_ = true;
            num_bytes_ = 8;
        }

        explicit Immediate(int32_t imm) : imm32_(imm)
        {
            is_set_ = true;
            num_bytes_ = 4;
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

    void Set32Imm(int32_t imm)
    {
        imm_ = Immediate(imm);
    }

    void Set64Imm(int64_t imm)
    {
        imm_ = Immediate(imm);
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
        }
    }

private:
    Opcode opcode_ {Opcode::INVALID};
    byte_t rd_ {0};
    byte_t rs1_ {0};
    byte_t rs2_ {0};
    Immediate imm_;
};

} // namespace evm::asm2byte

#endif // EVM_ASSEMBLER_ASM_TO_BYTE__INSTURCTION_H
