#ifndef EVM_RUNTIME_RUNTIME_H
#define EVM_RUNTIME_RUNTIME_H

#include "runtime/memory/garbage_collector/gc_incremental.h"
#include "runtime/memory/garbage_collector/gc_stw.h"
#include "runtime/memory/heap_manager.h"
#include "runtime/interpreter/interpreter.h"
#include "runtime/memory/class_manager.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace evm::file_format {
class File;
}

namespace evm::runtime {

class Runtime {
public:
    size_t DEFAULT_HEAP_SIZE = 32 * MBYTE_SIZE;

public:
    NO_COPY_SEMANTIC(Runtime);
    NO_MOVE_SEMANTIC(Runtime);

    static bool Create();
    static bool Destroy();

    static Runtime *GetInstance()
    {
        return instance_;
    }

    const HeapManager *GetHeapManager() const
    {
        return heap_manager_.get();
    }

    HeapManager *GetHeapManager()
    {
        return heap_manager_.get();
    }

    const Interpreter *GetInterpreter() const
    {
        return interpreter_.get();
    }

    Interpreter *GetInterpreter()
    {
        return interpreter_.get();
    }

    file_format::File *GetExecutableFile()
    {
        return file_;
    }

    const GarbageCollectorIncremental *GetGC() const
    {
        return gc_.get();
    }

    GarbageCollectorIncremental *GetGC()
    {
        return gc_.get();
    }

    void Execute(file_format::File *file);

    const std::string *GetStringFromCache(uint32_t string_offset);
    const std::string *CreateStringAndSetInCache(uint32_t string_offset);

    ClassManager *GetClassManager()
    {
        return &class_manager_;
    }

private:
    Runtime() = default;
    ~Runtime() = default;

    void InitializeRuntime();

private:
    static Runtime *instance_;

    std::unique_ptr<HeapManager> heap_manager_;
    std::unique_ptr<Interpreter> interpreter_;
    std::unique_ptr<GarbageCollectorIncremental> gc_;

    std::vector<byte_t> bytecode_;
    std::unordered_map<uint32_t, std::string> string_cache_;

    ClassManager class_manager_;

    file_format::File *file_ {nullptr};
};

} // namespace evm::runtime

#endif // EVM_RUNTIME_RUNTIME_H
