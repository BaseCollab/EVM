#include "interpreter.h"
#include "common/constants.h"

#include <iostream>

namespace evm {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

void Interpreter::Run(const byte_t *bytecode)
{
    static void *dispatch_table[] = { &&EXIT, &&ADD, &&SUB, &&MUL, &&DIV, &&AND, &&OR , &&XOR, &&MOVI, &&MOVR, &&CMPS, &&CMPU, &&PRINT, &&SCAN, &&INVALID };

    #define DISPATCH() \
        goto *dispatch_table[static_cast<byte_t>(bytecode[(pc_ += sizeof(insn_size_t))])];

    pc_ = 0;
    goto *dispatch_table[static_cast<byte_t>(bytecode[pc_])];

    EXIT:
        std::cerr << "EXIT" << std::endl;
        return;
    ADD:
        std::cerr << "ADD" << std::endl;
        DISPATCH();
    SUB:
        std::cerr << "SUB" << std::endl;
        DISPATCH();
    MUL:
        std::cerr << "MUL" << std::endl;
        DISPATCH();
    DIV:
        std::cerr << "DIV" << std::endl;
        DISPATCH();
    AND:
        std::cerr << "AND" << std::endl;
        DISPATCH();
    OR:
        std::cerr << "OR" << std::endl;
        DISPATCH();
    XOR:
        std::cerr << "XOR" << std::endl;
        DISPATCH();
    MOVI:
        std::cerr << "MOVI" << std::endl;
        DISPATCH();
    MOVR:
        std::cerr << "MOVR" << std::endl;
        DISPATCH();
    CMPS:
        std::cerr << "CMPS" << std::endl;
        DISPATCH();
    CMPU:
        std::cerr << "CMPU" << std::endl;
        DISPATCH();
    PRINT:
        std::cerr << "PRINT" << std::endl;
        DISPATCH();
    SCAN:
        std::cerr << "SCAN" << std::endl;
        DISPATCH();
    INVALID:
        std::cerr << "INVALID" << std::endl;
        exit(EXIT_FAILURE);
}

#pragma GCC diagnostic pop

} // namespace evm
