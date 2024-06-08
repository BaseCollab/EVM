#ifndef EVM_ASSEMBLER_ASM_TO_BYTE__INSTURCTION_H
#define EVM_ASSEMBLER_ASM_TO_BYTE__INSTURCTION_H

#include "common/utils/bitops.h"
#include "common/macros.h"
#include "common/constants.h"
#include "common/emittable.h"
#include "isa/opcodes.h"
#include "runtime/memory/frame.h"

#include <cassert>
#include <cstddef>
#include <vector>
#include <string>
#include <cstring>

namespace evm::file_format {

class Instruction : Emittable {
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

    Instruction(const std::string name, Opcode opcode) : Emittable(name), opcode_(opcode) {}

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

    /// Put hword_t number to rs1 and rs2 registers as little endian
    void SetClassOffset(hword_t rs12)
    {
        rs1_ = bitops::GetBits<bitops::BitSizeof<byte_t>() - 1, 0>(rs12);
        rs2_ = bitops::GetBits<2 * bitops::BitSizeof<byte_t>() - 1, bitops::BitSizeof<byte_t>()>(rs12);
    }

    void SetRs3(byte_t rs3)
    {
        rd_ = rs3;
    }

    void SetObjRs(byte_t obj_rs)
    {
        obj_rs_ = obj_rs;
        obj_op_ = true;
    }

    void SetObjFieldType(int8_t obj_field_type)
    {
        obj_field_type_ = obj_field_type;
        obj_op_ = true;
    }

    bool IsObjInstr() const
    {
        return obj_op_;
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

    void SetStringOp(const std::string str)
    {
        string_op_ = str;
    }

    const std::string &GetStringOp() const
    {
        return string_op_;
    }

    size_t GetImmSize() const
    {
        return imm_.num_bytes_;
    }

    void SetArg(size_t arg_num, byte_t arg_reg)
    {
        have_args_ = true;

        assert(arg_num < runtime::Frame::N_PASSED_ARGS_DEFAULT);
        args_[arg_num] = arg_reg;
    }

    void SetOffset(size_t offset)
    {
        bytcode_offset_ = offset;
    }

    size_t GetOffset()
    {
        return bytcode_offset_;
    }

    Opcode GetOpcode() const
    {
        return opcode_;
    }

    size_t GetBytesSize() const
    {
        return MINIMAL_INSTR_SIZE + imm_.num_bytes_ + (have_args_ == true) * runtime::Frame::N_PASSED_ARGS_DEFAULT;
    }

    EmitSize EmitBytecode(std::vector<byte_t> *out_arr)
    {
        out_arr->push_back(static_cast<byte_t>(opcode_));
        out_arr->push_back(rd_);
        out_arr->push_back(rs1_);
        out_arr->push_back(rs2_);

        if (obj_op_ == true) {
            out_arr->push_back(obj_rs_);
            out_arr->push_back(obj_field_type_);
        }

        if (imm_.is_set_) {
            size_t prev_size = out_arr->size();
            out_arr->insert(out_arr->end(), imm_.num_bytes_, 0);
            void *imm_ptr =
                (imm_.num_bytes_ == 4) ? static_cast<void *>(&(imm_.imm32_)) : static_cast<void *>(&(imm_.imm64_));
            std::memcpy(out_arr->data() + prev_size, imm_ptr, imm_.num_bytes_);
        } else if (have_args_ == true) {
            for (size_t i = 0; i < runtime::Frame::N_PASSED_ARGS_DEFAULT; ++i) {
                out_arr->push_back(args_[i]);
            }
        }

        return GetBytesSize();
    }

private:
    size_t bytcode_offset_;

    Opcode opcode_ {Opcode::INVALID};

    byte_t rd_ {0};
    byte_t rs1_ {0};
    byte_t rs2_ {0};

    /// No additional space for object-ptr encoding in register above
    byte_t obj_rs_ {0};
    int8_t obj_field_type_ {0};
    bool obj_op_ = false;

    Immediate imm_;

    std::string string_op_;

    bool have_args_ {false};
    byte_t args_[runtime::Frame::N_PASSED_ARGS_DEFAULT] = {0};
};

} // namespace evm::file_format

#endif // EVM_ASSEMBLER_ASM_TO_BYTE__INSTURCTION_H
