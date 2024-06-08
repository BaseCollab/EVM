#ifndef EVM_RUNTIME_INTERPRETER_H
#define EVM_RUNTIME_INTERPRETER_H

#include "common/macros.h"
#include "common/constants.h"
#include "runtime/memory/frame.h"
#include "runtime/memory/reg.h"
#include "isa/opcodes.h"

#include <cstddef>
#include <stack>

namespace evm::file_format {
class File;
} // namespace evm::file_format

namespace evm::runtime {

class Interpreter {
public:
    NO_COPY_SEMANTIC(Interpreter);
    NO_MOVE_SEMANTIC(Interpreter);

    Interpreter() = default;
    ~Interpreter() = default;

    void Run(file_format::File *file, const byte_t *bytecode, size_t entrypoint);
    const Frame *getCurrFrame() const;

private:
    std::stack<Frame> frames_;

    Frame *frame_cur_ {nullptr};
    size_t pc_ {0}; // pc of the current frame

    Register accum_;
};

} // namespace evm::runtime

#endif // EVM_RUNTIME_INTERPRETER_H
