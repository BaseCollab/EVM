#include "common/constants.h"
#include "common/config.h"
#include "runtime/interpreter/interpreter.h"
#include "runtime/memory/types/array.h"
#include "isa/macros.h"

#include <iostream>
#include <cstring>
#include <type_traits>
#include <cstdio>
#include <cmath>

namespace evm {

// Disable warning because the function uses computed goto
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-label-as-value"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

// clang-format off

#define DEBUG_LOG

#ifdef DEBUG_LOG
#define PRINT_DEBUG(name) std::cerr << #name << ", pc = " << pc_ << std::endl;
#else
#define PRINT_DEBUG(name)
#endif

#define RD_I_FRAME_ASSIGN(frame, value) frame->GetReg(RD_IDX())->SetInt64(value)
#define RD_F_FRAME_ASSIGN(frame, value) frame->GetReg(RD_IDX())->SetDouble(value)

#define RS1_I_FRAME(frame) frame->GetReg(RS1_IDX())->GetInt64()
#define RS1_F_FRAME(frame) frame->GetReg(RS1_IDX())->GetDouble()

#define RS2_I_FRAME(frame) frame->GetReg(RS2_IDX())->GetInt64()
#define RS2_F_FRAME(frame) frame->GetReg(RS2_IDX())->GetDouble()

#define RS3_I_FRAME(frame) frame->GetReg(RS3_IDX())->GetInt64()
#define RS3_F_FRAME(frame) frame->GetReg(RS3_IDX())->GetDouble()

void Interpreter::Run(const byte_t *bytecode)
{
    #define DEFINE_INSTR(instr, opcode, interpret) \
        &&instr,

    static void *dispatch_table[] = {
        #include "isa/isa.def"
    };

    #undef DEFINE_INSTR

    frames_.push(Frame(0, Frame::N_FRAME_LOCAL_REGS_DEFAULT));
    frame_cur_ = &frames_.top();

    size_t code_section_offset = 0;
    std::memcpy(&code_section_offset, bytecode, sizeof(code_section_offset));
    pc_ = code_section_offset;

    #define CALL_REG1() ISA_CALL_GET_REG1(bytecode + pc_)
    #define CALL_REG2() ISA_CALL_GET_REG2(bytecode + pc_)
    #define CALL_REG3() ISA_CALL_GET_REG3(bytecode + pc_)
    #define CALL_REG4() ISA_CALL_GET_REG4(bytecode + pc_)

    #define RD_IDX()    ISA_GET_RD (bytecode + pc_)
    #define RS1_IDX()   ISA_GET_RS1(bytecode + pc_)
    #define RS2_IDX()   ISA_GET_RS2(bytecode + pc_)
    #define RS3_IDX()   ISA_GET_RS3(bytecode + pc_)
    #define IMM_I()     ISA_GET_IMM(bytecode + pc_, int64_t)
    #define IMM_F()     ISA_GET_IMM(bytecode + pc_, double)
    #define IMM_I32()   ISA_GET_IMM(bytecode + pc_, int32_t)

    #define BYTECODE_OFFSET(offset) bytecode + offset

    #define PC()             pc_
    #define PC_ADD(value)    pc_ += value
    #define PC_ASSIGN(value) pc_ =  value

    #define RD_I_ASSIGN(value) RD_I_FRAME_ASSIGN(frame_cur_, value)
    #define RD_F_ASSIGN(value) RD_F_FRAME_ASSIGN(frame_cur_, value)

    #define RS1_I() RS1_I_FRAME(frame_cur_)
    #define RS1_F() RS1_F_FRAME(frame_cur_)

    #define RS2_I() RS2_I_FRAME(frame_cur_)
    #define RS2_F() RS2_F_FRAME(frame_cur_)

    #define RS3_I() RS3_I_FRAME(frame_cur_)
    #define RS3_F() RS3_F_FRAME(frame_cur_)

    #define PUT_ACCUM(reg)     accum_ = reg
    #define PUT_I_ACCUM(value) accum_.SetInt64(value)
    #define PUT_F_ACCUM(value) accum_.SetDouble(value)

    #define GET_ACCUM()   accum_
    #define GET_I_ACCUM() accum_.GetInt64()
    #define GET_F_ACCUM() accum_.GetDouble()

    #define GET_ARRAY_TYPE() ISA_GET_TYPE(bytecode + pc_)

    #define CREATE_ARR(type, size) \
        reinterpret_cast<int64_t>(memory::Array::Create(*vm_, static_cast<memory::Array::Type>(type), size))

    #define LOAD_FROM_ARR(array_ptr, idx)                                    \
    ({                                                                       \
        memory::Array *array = reinterpret_cast<memory::Array *>(array_ptr); \
        int64_t value = 0;                                                   \
        array->Get(&value, idx);                                             \
        value;                                                               \
    })

    #define STORE_TO_ARR(array_ptr, array_idx, src_reg)                      \
        memory::Array *array = reinterpret_cast<memory::Array *>(array_ptr); \
        array->Set(src_reg, array_idx);

    #define FRAME_NEW_MIGRATE(restore_pc, new_pc)                            \
        frame_cur_->SetRestorePC(restore_pc);                                \
        frames_.push(Frame(new_pc, Frame::N_FRAME_LOCAL_REGS_DEFAULT,        \
                     *frame_cur_->GetReg(CALL_REG1()),                       \
                     *frame_cur_->GetReg(CALL_REG2()),                       \
                     *frame_cur_->GetReg(CALL_REG3()),                       \
                     *frame_cur_->GetReg(CALL_REG4())));                     \
        PC_ASSIGN(new_pc);                                                   \
        frame_cur_ = &frames_.top();

    #define FRAME_OLD_MIGRATE()                                              \
        frames_.pop();                                                       \
        frame_cur_ = &frames_.top();                                         \
        PC_ASSIGN(frame_cur_->GetRestorePC());

    #define DISPATCH() goto *dispatch_table[static_cast<byte_t>(bytecode[(pc_)])];

    goto *dispatch_table[static_cast<byte_t>(bytecode[pc_])];

    #define DEFINE_INSTR(instr, opcode, interpret)    \
    instr:                                            \
        PRINT_DEBUG(instr);                           \
        interpret;                                    \
        DISPATCH();

    #include "isa/isa.def"

    #undef DEFINE_INSTR
}

const Frame *Interpreter::getCurrFrame() const
{
    return frame_cur_;
}

#pragma GCC diagnostic pop

// clang-format on

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

} // namespace evm
