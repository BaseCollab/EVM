#ifndef EVM_INTERPRETER_H
#define EVM_INTERPRETER_H

#include "common/macros.h"
#include "common/constants.h"
#include "frame.h"
#include "isa/opcodes.h"

#include <cstddef>
#include <stack>

namespace evm {

class VirtualMachine;

class Interpreter {
public:
    NO_COPY_SEMANTIC(Interpreter);
    NO_MOVE_SEMANTIC(Interpreter);

    Interpreter() = default;
    ~Interpreter() = default;

    void Run(const byte_t *bytecode);
    const Frame *getCurrFrame() const;

private:
    std::stack<Frame> frames_;

    Frame *frame_cur_ {nullptr};
    size_t pc_ {0}; // pc of the current frame

#endif // EVM_INTERPRETER_H
