#include "runtime/interpreter/interpreter.h"
#include "common/constants.h"
#include "common/config.h"
#include "runtime/interpreter/interpreter-inl.h"
#include "runtime/memory/reg.h"
#include "runtime/memory/types/array.h"
#include "file_format/file.h"
#include "isa/macros.h"

#include <iostream>
#include <cstring>
#include <type_traits>
#include <cstdio>
#include <cmath>

namespace evm::runtime {

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

#define RD_MARK_REG_AS_ROOT(frame, is_obj) \
    frame->MarkReg(RD_IDX(), is_obj)

#define RS1_IS_MARKED_AS_ROOT(frame) \
    frame->IsRegMarked(RS1_IDX())

void Interpreter::Run(file_format::File *file, const byte_t *bytecode, size_t entrypoint)
{
    #define DEFINE_INSTR(instr, opcode, interpret) \
        &&instr,

    static void *dispatch_table[] = {
        #include "isa/isa.def"
    };

    #undef DEFINE_INSTR

    frames_.emplace_back(Frame(0, {}));
    frame_cur_ = &frames_.back();

    pc_ = entrypoint;

    #define CALL_REG1()          *frame_cur_->GetReg(ISA_CALL_GET_REG1(bytecode + pc_))
    #define CALL_REG2()          *frame_cur_->GetReg(ISA_CALL_GET_REG2(bytecode + pc_))
    #define CALL_REG3()          *frame_cur_->GetReg(ISA_CALL_GET_REG3(bytecode + pc_))
    #define CALL_REG4()          *frame_cur_->GetReg(ISA_CALL_GET_REG4(bytecode + pc_))

    #define RD_IDX()             ISA_GET_RD (bytecode + pc_)
    #define RS1_IDX()            ISA_GET_RS1(bytecode + pc_)
    #define RS2_IDX()            ISA_GET_RS2(bytecode + pc_)
    #define RS3_IDX()            ISA_GET_RS3(bytecode + pc_)
    #define IMM_I()              ISA_GET_IMM(bytecode + pc_, int64_t)
    #define IMM_F()              ISA_GET_IMM(bytecode + pc_, double)
    #define IMM_I32()            ISA_GET_IMM(bytecode + pc_, int32_t)

    #define GET_ARRAY_TYPE()     ISA_GET_ARRAY_TYPE(bytecode + pc_)
    #define GET_OBJ_TYPE()       ISA_GET_OBJ_TYPE(bytecode + pc_)
    #define GET_OBJ_FIELD_IDX()  ISA_GET_OBJ_TYPE(bytecode + pc_)
    #define GET_OBJ_RS()         ISA_GET_OBJ_RS(bytecode + pc_)
    #define GET_OBJ_OP_RS()      ISA_GET_OBJ_OP_RS(bytecode + pc_)
    #define GET_OBJ_FIELD_TYPE() ISA_GET_OBJ_FIELD_TYPE(bytecode + pc_)

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

    #define MARK_RD_AS_ROOT(is_obj) \
        RD_MARK_REG_AS_ROOT(frame_cur_, is_obj)

    #define IS_RS1_MARKED_AS_ROOT() \
        RS1_IS_MARKED_AS_ROOT(frame_cur_)

    #define CHECK_GC_INVOKE() \
        Runtime::GetInstance()->GetGC()->UpdateState();

    #define DISPATCH() \
        goto *dispatch_table[static_cast<byte_t>(bytecode[(pc_)])];

    goto *dispatch_table[static_cast<byte_t>(bytecode[pc_])];

    #define DEFINE_INSTR(instr, opcode, interpret)    \
    instr:                                            \
    {                                                 \
        PRINT_DEBUG(instr);                           \
        interpret;                                    \
        CHECK_GC_INVOKE();                            \
        DISPATCH();                                   \
    }

    #include "isa/isa.def"

    #undef DEFINE_INSTR
}

const std::vector<Frame> &Interpreter::GetFramesStack() const
{
    return frames_;
}

const Frame *Interpreter::GetCurrFrame() const
{
    return frame_cur_;
}

void Interpreter::MigrateToNewFrame(size_t new_pc, size_t restore_pc,
                                    const std::array<Register, Frame::N_PASSED_ARGS_DEFAULT> &passed_args)
{
    frame_cur_->SetRestorePC(restore_pc);
    frames_.emplace_back(Frame(new_pc, passed_args));
    pc_ = new_pc;
    frame_cur_ = &frames_.back();
}

void Interpreter::ReturnToPrevFrame()
{
    frames_.pop_back();
    frame_cur_ = &frames_.back();
    pc_ = frame_cur_->GetRestorePC();
}

void Interpreter::MarkAccum(bool is_root)
{
    is_accum_root_ = is_root;
}

bool Interpreter::IsAccumMarked() const
{
    return is_accum_root_;
}

Register Interpreter::GetAccum() const
{
    return accum_;
}

#pragma GCC diagnostic pop

// clang-format on

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

} // namespace evm::runtime
