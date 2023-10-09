#include "common/constants.h"
#include "interpreter.h"
#include "vm/vm.h"

#include <iostream>
#include <cstring>
#include <type_traits>
#include <cstdio>

namespace evm {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

#ifdef N_DEBUG
#define PRINT_DEBUG(name)
#else
#define PRINT_DEBUG(name) std::cerr << #name << std::endl;
#endif

#define GET_RD() bytecode[pc_ + 1]

#define GET_RS1() bytecode[pc_ + 2]

#define GET_RS2() bytecode[pc_ + 3]

#define GET_IMM()                                                                 \
    ({                                                                            \
        reg_t val_ = 0;                                                           \
        std::memcpy(&val_, bytecode + pc_ + sizeof(insn_size_t), sizeof(reg_t));  \
        pc_ += sizeof(reg_t);                                                     \
        val_;                                                                     \
    })

void Interpreter::Run(VirtualMachine *vm, const byte_t *bytecode)
{
    static void *dispatch_table[] = {&&EXIT, &&ADD,  &&SUB,  &&MUL,  &&DIV, &&AND,  &&OR,  &&XOR,
                                     &&MULU, &&DIVU, &&MOVI, &&MOVR, &&SLT, &&SLTU, &&SME, &&SMEU, &&EQ, &&NEQ,
                                     &&PRINT, &&PRINTU, &&SCAN, &&INVALID};

#define DISPATCH() goto *dispatch_table[static_cast<byte_t>(bytecode[(pc_ += sizeof(insn_size_t))])];

    reg_idx_t reg_idx = -1;
    reg_t value = 0;

    pc_ = 0;
    goto *dispatch_table[static_cast<byte_t>(bytecode[pc_])];

EXIT:
    PRINT_DEBUG(EXIT);
    return;
ADD:
    PRINT_DEBUG(ADD);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()) + vm->GetReg(GET_RS2()));

    DISPATCH();
SUB:
    PRINT_DEBUG(SUB);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()) - vm->GetReg(GET_RS2()));

    DISPATCH();
MUL:
    PRINT_DEBUG(MUL);

    vm->SetReg(GET_RD(), static_cast<std::make_signed_t<reg_t>>(vm->GetReg(GET_RS1())) *
                         static_cast<std::make_signed_t<reg_t>>(vm->GetReg(GET_RS2())));

    DISPATCH();
DIV:
    PRINT_DEBUG(DIV);

    vm->SetReg(GET_RD(), static_cast<std::make_signed_t<reg_t>>(vm->GetReg(GET_RS1())) /
                         static_cast<std::make_signed_t<reg_t>>(vm->GetReg(GET_RS2())));

    DISPATCH();
AND:
    PRINT_DEBUG(AND);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()) & vm->GetReg(GET_RS2()));

    DISPATCH();
OR:
    PRINT_DEBUG(OR);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()) | vm->GetReg(GET_RS2()));

    DISPATCH();
XOR:
    PRINT_DEBUG(XOR);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()) ^ vm->GetReg(GET_RS2()));

    DISPATCH();
MULU:
    PRINT_DEBUG(MULU);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()) * vm->GetReg(GET_RS2()));

    DISPATCH();
DIVU:
    PRINT_DEBUG(DIVU);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()) / vm->GetReg(GET_RS2()));

    DISPATCH();
MOVI:
    PRINT_DEBUG(MOVI);

    reg_idx = GET_RD();
    value = GET_IMM();
    vm->SetReg(reg_idx, value);

    DISPATCH();
MOVR:
    PRINT_DEBUG(MOVR);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()));

    DISPATCH();
SLT:
    PRINT_DEBUG(SLTS);

    vm->SetReg(GET_RD(), static_cast<std::make_signed_t<reg_t>>(vm->GetReg(GET_RS1())) <
                         static_cast<std::make_signed_t<reg_t>>(vm->GetReg(GET_RS2())));

    DISPATCH();
SLTU:
    PRINT_DEBUG(SLTU);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()) < vm->GetReg(GET_RS2()));

    DISPATCH();
SME:
    PRINT_DEBUG(SMES);

    vm->SetReg(GET_RD(), static_cast<std::make_signed_t<reg_t>>(vm->GetReg(GET_RS1())) >=
                         static_cast<std::make_signed_t<reg_t>>(vm->GetReg(GET_RS2())));

    DISPATCH();
SMEU:
    PRINT_DEBUG(SMEU);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()) >= vm->GetReg(GET_RS2()));

    DISPATCH();
EQ:
    PRINT_DEBUG(EQ);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()) == vm->GetReg(GET_RS2()));

    DISPATCH();
NEQ:
    PRINT_DEBUG(EQ);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()) != vm->GetReg(GET_RS2()));

    DISPATCH();
PRINT:
    PRINT_DEBUG(PRINT);

    printf("%ld\n", static_cast<std::make_signed_t<reg_t>>(vm->GetReg(GET_RS1())));

    DISPATCH();
PRINTU:
    PRINT_DEBUG(PRINT);

    printf("%lu\n", vm->GetReg(GET_RS1()));

    DISPATCH();
SCAN:
    PRINT_DEBUG(SCAN);

    scanf("%lu", &value);
    vm->SetReg(GET_RD(), value);

    DISPATCH();
INVALID:
    PRINT_DEBUG(INVALID);
    exit(EXIT_FAILURE);
}

#pragma GCC diagnostic pop

} // namespace evm
