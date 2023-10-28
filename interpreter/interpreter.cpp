#include "common/constants.h"
#include "interpreter.h"
#include "vm/vm.h"

#include <iostream>
#include <cstring>
#include <type_traits>
#include <cstdio>
#include <cmath>

namespace evm {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

#ifdef DEBUG_LOG
#define PRINT_DEBUG(name) std::cerr << #name << std::endl;
#else
#define PRINT_DEBUG(name)
#endif

#define GET_RD() bytecode[pc_ + 1]

#define GET_RS1() bytecode[pc_ + 2]

#define GET_RS2() bytecode[pc_ + 3]

#define GET_IMM(type)                                                            \
    ({                                                                           \
        type __val = 0;                                                          \
        std::memcpy(&__val, bytecode + pc_ + sizeof(insn_size_t), sizeof(type)); \
        pc_ += sizeof(type);                                                     \
        __val;                                                                   \
    })

void Interpreter::Run(VirtualMachine *vm, const byte_t *bytecode)
{
    static void *dispatch_table[] = {
        &&EXIT,   &&ADD,    &&SUB,   &&MUL,   &&DIV,   &&AND,  &&OR,      &&XOR,     &&MULU,    &&DIVU,    &&MOVR,
        &&MOVI,   &&MOVF,   &&MOVFR, &&SLT,   &&SLTU,  &&SME,  &&SMEU,    &&EQ,      &&NEQ,     &&ADDF,    &&SUBF,
        &&MULF,   &&DIVF,   &&SLTF,  &&SMEF,  &&EQF,   &&NEQF, &&CONVRSF, &&CONVFRS, &&CONVRUF, &&CONVFRU, &&PRINT,
        &&PRINTU, &&PRINTF, &&SCAN,  &&SCANU, &&SCANF, &&SIN,  &&COS,     &&INVALID};

#define DISPATCH() goto *dispatch_table[static_cast<byte_t>(bytecode[(pc_ += sizeof(insn_size_t))])];

    reg_idx_t reg_idx = -1;

    reg_t reg_value = 0;
    std::make_signed_t<reg_t> ireg_val = 0;
    double freg_value = 0;

    pc_ = 0;
    goto *dispatch_table[static_cast<byte_t>(bytecode[pc_])];

    #define RD_ASSIGN(value) vm->SetReg(GET_RD(), value);

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
MOVR:
    PRINT_DEBUG(MOVR);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()));

    DISPATCH();
MOVI:
    PRINT_DEBUG(MOVI);

    reg_idx = GET_RD();
    reg_value = GET_IMM(reg_t);
    vm->SetReg(reg_idx, reg_value);

    DISPATCH();
MOVF:
    PRINT_DEBUG(MOVF);

    vm->SetFReg(GET_RD(), vm->GetFReg(GET_RS1()));

    DISPATCH();
MOVFR:
    PRINT_DEBUG(MOVFR);

    reg_idx = GET_RD();
    freg_value = GET_IMM(freg_t);
    vm->SetFReg(reg_idx, freg_value);

    DISPATCH();
SLT:
    PRINT_DEBUG(SLT);

    vm->SetReg(GET_RD(), static_cast<std::make_signed_t<reg_t>>(vm->GetReg(GET_RS1())) <
                             static_cast<std::make_signed_t<reg_t>>(vm->GetReg(GET_RS2())));

    DISPATCH();
SLTU:
    PRINT_DEBUG(SLTU);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()) < vm->GetReg(GET_RS2()));

    DISPATCH();
SME:
    PRINT_DEBUG(SME);

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
ADDF:
    PRINT_DEBUG(ADDF);

    vm->SetFReg(GET_RD(), vm->GetFReg(GET_RS1()) + vm->GetFReg(GET_RS2()));

    DISPATCH();
SUBF:
    PRINT_DEBUG(SUBF);

    vm->SetFReg(GET_RD(), vm->GetFReg(GET_RS1()) - vm->GetFReg(GET_RS2()));

    DISPATCH();
MULF:
    PRINT_DEBUG(MULF);

    vm->SetFReg(GET_RD(), vm->GetFReg(GET_RS1()) * vm->GetFReg(GET_RS2()));

    DISPATCH();
DIVF:
    PRINT_DEBUG(DIVF);

    vm->SetFReg(GET_RD(), vm->GetFReg(GET_RS1()) / vm->GetFReg(GET_RS2()));

    DISPATCH();
SLTF:
    PRINT_DEBUG(SLTF);

    vm->SetReg(GET_RD(), vm->GetFReg(GET_RS1()) < vm->GetFReg(GET_RS2()));

    DISPATCH();
SMEF:
    PRINT_DEBUG(SMEF);

    vm->SetReg(GET_RD(), vm->GetFReg(GET_RS1()) >= vm->GetFReg(GET_RS2()));

    DISPATCH();
EQF:
    PRINT_DEBUG(EQF);

    vm->SetReg(GET_RD(), vm->GetFReg(GET_RS1()) == vm->GetFReg(GET_RS2()));

    DISPATCH();
NEQF:
    PRINT_DEBUG(NEQF);

    vm->SetReg(GET_RD(), vm->GetFReg(GET_RS1()) != vm->GetFReg(GET_RS2()));

    DISPATCH();
CONVRSF:
    PRINT_DEBUG(CONVRSF);

    vm->SetFReg(GET_RD(), static_cast<freg_t>(static_cast<std::make_signed_t<reg_t>>(vm->GetReg(GET_RS1()))));

    DISPATCH();
CONVFRS:
    PRINT_DEBUG(CONVFRS);

    vm->SetReg(GET_RD(), static_cast<reg_t>(static_cast<std::make_signed_t<reg_t>>(vm->GetFReg(GET_RS1()))));

    DISPATCH();
CONVRUF:
    PRINT_DEBUG(CONVRUF);

    vm->SetFReg(GET_RD(), static_cast<freg_t>(vm->GetReg(GET_RS1())));

    DISPATCH();
CONVFRU:
    PRINT_DEBUG(CONVFRU);

    vm->SetReg(GET_RD(), static_cast<reg_t>(vm->GetFReg(GET_RS1())));

    DISPATCH();
PRINT:
    PRINT_DEBUG(PRINT);

    printf("%ld\n", static_cast<std::make_signed_t<reg_t>>(vm->GetReg(GET_RS1())));

    DISPATCH();
PRINTU:
    PRINT_DEBUG(PRINTU);

    printf("%lu\n", vm->GetReg(GET_RS1()));

    DISPATCH();
PRINTF:
    PRINT_DEBUG(PRINTF);
    printf("%lf\n", vm->GetFReg(GET_RS1()));

    DISPATCH();
SCAN:
    PRINT_DEBUG(SCAN);

    scanf("%ld", &ireg_val);
    std::memcpy(&reg_value, &ireg_val, sizeof(ireg_val));
    vm->SetReg(GET_RD(), reg_value);

    DISPATCH();
SCANU:
    PRINT_DEBUG(SCANU);

    scanf("%lu", &reg_value);
    vm->SetReg(GET_RD(), reg_value);

    DISPATCH();
SCANF:
    PRINT_DEBUG(SCANF);

    scanf("%lf", &freg_value);
    vm->SetFReg(GET_RD(), freg_value);

    DISPATCH();
SIN:
    PRINT_DEBUG(SIN);

    vm->SetFReg(GET_RD(), std::sin(vm->GetFReg(GET_RS1())));

    DISPATCH();
COS:
    PRINT_DEBUG(COS);

    vm->SetFReg(GET_RD(), std::cos(vm->GetFReg(GET_RS1())));

    DISPATCH();
INVALID:
    PRINT_DEBUG(INVALID);
    exit(EXIT_FAILURE);
}

#pragma GCC diagnostic pop

} // namespace evm
