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

#define GET_IMM()                                                               \
    ({                                                                          \
        uint64_t val = 0;                                                       \
        std::memcpy(&val, bytecode + pc_ + sizeof(insn_size_t), sizeof(reg_t)); \
        pc_ += sizeof(reg_t);                                                   \
        val;                                                                    \
    })

void Interpreter::Run(VirtualMachine *vm, const byte_t *bytecode)
{
    static void *dispatch_table[] = {&&EXIT, &&ADD,  &&SUB,  &&MUL,  &&DIV,  &&AND, &&OR,    &&XOR,  &&MOVI,
                                     &&MOVR, &&SLTS, &&SLTU, &&SMES, &&SMEU, &&EQ,  &&PRINT, &&SCAN, &&INVALID};

#define DISPATCH() goto *dispatch_table[static_cast<byte_t>(bytecode[(pc_ += sizeof(insn_size_t))])];

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

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()) * vm->GetReg(GET_RS2()));

    DISPATCH();
DIV:
    PRINT_DEBUG(DIV);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()) / vm->GetReg(GET_RS2()));

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
MOVI:
    PRINT_DEBUG(MOVI);

    vm->SetReg(GET_RD(), GET_IMM());

    DISPATCH();
MOVR:
    PRINT_DEBUG(MOVR);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()));

    DISPATCH();
SLTS:
    PRINT_DEBUG(SLTS);

    vm->SetReg(GET_RD(), static_cast<std::make_signed_t<reg_t>>(vm->GetReg(GET_RS1())) <
                             static_cast<std::make_signed_t<reg_t>>(vm->GetReg(GET_RS2())));

    DISPATCH();
SLTU:
    PRINT_DEBUG(SLTU);

    vm->SetReg(GET_RD(), vm->GetReg(GET_RS1()) < vm->GetReg(GET_RS2()));

    DISPATCH();
SMES:
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
PRINT:
    PRINT_DEBUG(PRINT);

    printf("%lu\n", vm->GetReg(GET_RS1()));

    DISPATCH();
SCAN:
    PRINT_DEBUG(SCAN);

    reg_t val = 0;
    scanf("%lu", &val);
    vm->SetReg(GET_RD(), val);

    DISPATCH();
INVALID:
    PRINT_DEBUG(INVALID);
    exit(EXIT_FAILURE);
}

#pragma GCC diagnostic pop

} // namespace evm
