#include "runtime/runtime.h"
#include "file_format/file.h"
#include "runtime/memory/garbage_collector/gc_stw.h"

#include <iostream>

namespace evm::runtime {

Runtime *Runtime::instance_ = nullptr;

/* static */
bool Runtime::Create()
{
    if (instance_ != nullptr) {
        std::cerr << "Runtime already exists." << std::endl;
        return false;
    }

    instance_ = new Runtime();
    if (instance_ == nullptr) {
        std::cerr << "Error when allocating runtime." << std::endl;
        return false;
    }

    instance_->InitializeRuntime();

    return true;
}

/* static */
bool Runtime::Destroy()
{
    if (instance_ == nullptr) {
        return false;
    }

    delete instance_;
    instance_ = nullptr;

    return true;
}

void Runtime::InitializeRuntime()
{
    heap_manager_ = std::make_unique<HeapManager>(DEFAULT_HEAP_SIZE);
    interpreter_ = std::make_unique<Interpreter>();

    class_manager_.InitDefaultClassDescriptions();
    gc_ = std::make_unique<GarbageCollectorIncremental>();
}

void Runtime::Execute(file_format::File *file)
{
    assert(file != nullptr);

    file_ = file;
    file->EmitBytecode(&bytecode_);

    size_t entrypoint = file->GetCodeSection()->GetOffset();
    interpreter_->Run(file, bytecode_.data(), entrypoint);
}

const std::string *Runtime::GetStringFromCache(uint32_t string_offset)
{
    if (auto find = string_cache_.find(string_offset); find != string_cache_.end()) {
        return &(find->second);
    }
    return nullptr;
}

const std::string *Runtime::CreateStringAndSetInCache(uint32_t string_offset)
{
    // Strings in bytecode are null-terminated.
    std::string str(reinterpret_cast<char *>(bytecode_.data()) + string_offset);

    auto pair = string_cache_.insert({string_offset, std::move(str)});
    auto iter = pair.first;

    return &(iter->second);
}

} // namespace evm::runtime
