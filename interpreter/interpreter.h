#ifndef EVM_INTERPRETER_H
#define EVM_INTERPRETER_H

#include "common/macros.h"
#include "common/constants.h"
#include "memory/frame.h"
#include "memory/reg.h"
#include "isa/opcodes.h"

#include <cstddef>
#include <stack>

namespace evm {

class VirtualMachine;

class Interpreter {
public:
    NO_COPY_SEMANTIC(Interpreter);
    NO_MOVE_SEMANTIC(Interpreter);

    Interpreter(VirtualMachine *vm) : vm_(vm) {}
    ~Interpreter() = default;

    void Run(const byte_t *bytecode);
    const Frame *getCurrFrame() const;

private:
    VirtualMachine *vm_ {nullptr};

    std::stack<Frame> frames_;

    Frame *frame_cur_ {nullptr};
    size_t pc_ {0}; // pc of the current frame

    Register accum_;
};

} // namespace evm

#endif // EVM_INTERPRETER_H
