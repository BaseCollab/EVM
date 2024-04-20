#ifndef EVM_RUNTIME_RUNTIME_H
#define EVM_RUNTIME_RUNTIME_H

#include "common/macros.h"

namespace evm::runtime {

class ClassLinker;

class Runtime {
public:
    NO_COPY_SEMANTIC(Runtime);
    NO_MOVE_SEMANTIC(Runtime);

    static void Create();

    ClassLinker *GetClassLinker() const
    {
        return class_linker_;
    }

private:
    Runtime() = default;
    ~Runtime() = default;

private:
    ClassLinker* class_linker_ {nullptr};
};

} // namespace evm::runtime


#endif // EVM_RUNTIME_RUNTIME_H
