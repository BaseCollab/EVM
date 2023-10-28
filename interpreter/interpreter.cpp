#include "common/constants.h"
#include "interpreter.h"
#include "isa/macros.h"

#include <iostream>
#include <cstring>
#include <type_traits>
#include <cstdio>
#include <cmath>

namespace evm {

// clang-format off

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

#define DEBUG_LOG

#ifdef DEBUG_LOG
#define PRINT_DEBUG(name) std::cerr << #name << std::endl;
#else
#define PRINT_DEBUG(name)
#endif

#define RD_I_FRAME_ASSIGN(frame, value) frame->GetReg(RD_IDX())->SetInt64(value)
#define RD_F_FRAME_ASSIGN(frame, value) frame->GetReg(RD_IDX())->SetDouble(value)

#define RS1_I_FRAME(frame) frame->GetReg(RS1_IDX())->GetInt64()
#define RS1_F_FRAME(frame) frame->GetReg(RS1_IDX())->GetDouble()

#define RS2_I_FRAME(frame) frame->GetReg(RS2_IDX())->GetInt64()
#define RS2_F_FRAME(frame) frame->GetReg(RS2_IDX())->GetDouble()

void Interpreter::Run(const byte_t *bytecode)
{
    #define DEFINE_INSTR(instr, opcode, imm_len, interpret) \
        &&instr,

    static void *dispatch_table[] = {
        #include "isa/isa.def"
    };

    #undef DEFINE_INSTR

    frames_.push(Frame(0, Frame::N_FRAME_LOCAL_REGS_DEFAULT));
    frame_cur_ = &frames_.top();
    pc_ = 0;

    #define RD_IDX()  ISA_GET_RD (bytecode + pc_)
    #define RS1_IDX() ISA_GET_RS1(bytecode + pc_)
    #define RS2_IDX() ISA_GET_RS2(bytecode + pc_)
    #define IMM_I()   ISA_GET_IMM(bytecode + pc_, int64_t)
    #define IMM_F()   ISA_GET_IMM(bytecode + pc_, double)

    #define RD_I_ASSIGN(value) RD_I_FRAME_ASSIGN(frame_cur_, value)
    #define RD_F_ASSIGN(value) RD_F_FRAME_ASSIGN(frame_cur_, value)

    #define RS1_I() RS1_I_FRAME(frame_cur_)
    #define RS1_F() RS1_F_FRAME(frame_cur_)

    #define RS2_I() RS2_I_FRAME(frame_cur_)
    #define RS2_F() RS2_F_FRAME(frame_cur_)

    #define DISPATCH() goto *dispatch_table[static_cast<byte_t>(bytecode[(pc_)])];

    goto *dispatch_table[static_cast<byte_t>(bytecode[pc_])];

    #define DEFINE_INSTR(instr, opcode, imm_len, interpret) \
    instr:                                                  \
        PRINT_DEBUG(instr);                                 \
        interpret;                                          \
        pc_ = ISA_NEXT_INSTR(pc_) + imm_len;                \
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

} // namespace evm
