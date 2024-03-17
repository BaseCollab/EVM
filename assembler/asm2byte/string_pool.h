#ifndef EVM_ASSEMBLER_ASM_TO_BYTE__STRING_POOL_H
#define EVM_ASSEMBLER_ASM_TO_BYTE__STRING_POOL_H

#include "common/macros.h"
#include "common/constants.h"
#include "instruction.h"
#include "emittable.h"

#include <vector>
#include <cstring>
#include <unordered_map>
#include <stack>

namespace evm::asm2byte {

class StringPool : Emittable {
public:
    DEFAULT_MOVE_SEMANTIC(StringPool);
    DEFAULT_COPY_SEMANTIC(StringPool);

    StringPool(const std::string name, EmitRef offset = 0) :
        Emittable(name),
        offset_(offset)
    {}

    ~StringPool() = default;

    void SetOffset(EmitRef offset)
    {
        offset_ = offset;
    }

    EmitRef GetOffset() const
    {
        return offset_;
    }

    bool HasInstance(const std::string &str) const
    {
        auto string_pos = string_pull_.find(str);
        return string_pos != string_pull_.end();
    }

    void AddInstance(const std::string &str)
    {
        string_pull_[str] = size_;
        size_ += std::strlen(str.c_str()) + 1;
    }

    void AddInstrToResolve(Instruction *instr)
    {
        resolution_table_.push(instr);
    }

    bool ResolveInstrs()
    {
        if (!offset_) {
            return false;
        }

        while (!resolution_table_.empty()) {
            Instruction *to_resolve = resolution_table_.top();
            to_resolve->Set32Imm(string_pull_[to_resolve->GetStringOp()] + offset_);
            resolution_table_.pop();
        }

        return true;
    }

    EmitSize EmitBytecode(std::vector<byte_t> *out_arr)
    {

    }

    size_t GetSize() const
    {
        return size_;
    }

private:
    std::unordered_map<std::string, size_t> string_pull_;
    std::stack<Instruction *> resolution_table_;

    size_t size_ = 0;
    EmitRef offset_ = 0;
}

} // namespace evm::asm2byte

#endif // EVM_ASSEMBLER_ASM_TO_BYTE__STRING_POOL_H
