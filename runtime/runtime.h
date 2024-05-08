#ifndef EVM_RUNTIME_RUNTIME_H
#define EVM_RUNTIME_RUNTIME_H

#include "runtime/memory/heap_manager.h"

namespace evm::runtime {

class Runtime {
public:
    size_t DEFAULT_HEAP_SIZE = 32 * MBYTE_SIZE;

public:
    NO_COPY_SEMANTIC(Runtime);
    NO_MOVE_SEMANTIC(Runtime);

    static bool Create();
    static bool Destroy();

    HeapManager *GetHeapManager() const
    {
        return heap_manager_.get();
    }

    static Runtime *GetInstance() const
    {
        return instance_;
    }

    static ExecuteFile();

private:
    Runtime() = default;
    ~Runtime() = default;
    
private:
    static Runtime *instance_;

    std::unique_ptr<HeapManager> heap_manager_;
    std::unique_ptr<Interpreter> interpreter_
};

} // namespace evm::runtime


#endif // EVM_RUNTIME_RUNTIME_H
