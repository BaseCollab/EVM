#ifndef EVM_RUNTIME_INTERPRETER_H
#define EVM_RUNTIME_INTERPRETER_H

#include "common/macros.h"
#include "common/constants.h"
#include "runtime/memory/frame.h"
#include "runtime/memory/reg.h"

#include <cstddef>
#include <vector>

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
    const Frame *GetCurrFrame() const;
    const std::vector<Frame> &GetFramesStack() const;

    void MigrateToNewFrame(size_t new_pc, size_t restore_pc,
                           const std::array<Register, Frame::N_PASSED_ARGS_DEFAULT> &passed_args);
    void ReturnToPrevFrame();

private:
    std::vector<Frame> frames_;

    Frame *frame_cur_ {nullptr};
    size_t pc_ {0}; // pc of the current frame

    Register accum_;
};

} // namespace evm::runtime

#endif // EVM_RUNTIME_INTERPRETER_H
