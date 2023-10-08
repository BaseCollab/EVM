#ifndef EVM_INTERPRETER_H
#define EVM_INTERPRETER_H

#include "common/macros.h"
#include "common/constants.h"
#include "isa/opcodes.h"

#include <cstddef>

namespace evm {

class VirtualMachine;

class Interpreter {
public:
    NO_COPY_SEMANTIC(Interpreter);
    NO_MOVE_SEMANTIC(Interpreter);

    Interpreter() = default;
    ~Interpreter() = default;

    void Run(VirtualMachine *vm, const byte_t *bytecode);

private:
    size_t pc_ {0};
};

} // namespace evm

#endif // EVM_INTERPRETER_H
