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

    size_t GetStringsStart() const
    {
        return offset_ + Emittable::GetSize();
    }

    size_t GetSize() const
    {
        return Emittable::GetSize() + size_;
    }

    bool HasInstance(const std::string &str) const
    {
        auto string_pos = string_pull_.find(str);
        return string_pos != string_pull_.end();
    }

    void AddInstance(const std::string &str)
    {
        string_pull_[str] = size_;
        size_ += str.size() + 1;
    }

    void AddInstrToResolve(Instruction *instr)
    {
        resolution_table_.push(instr);
    }

    bool ResolveInstrs()
    {
        // Not set offset => don't know real offset of string literals => cannot resolve instsrs
        if (!offset_) {
            return false;
        }

        while (!resolution_table_.empty()) {
            Instruction *to_resolve = resolution_table_.top();
            to_resolve->Set32Imm(string_pull_[to_resolve->GetStringOp()] + GetStringsStart());
            resolution_table_.pop();
        }

        return true;
    }

    EmitSize EmitBytecode(std::vector<byte_t> *out_arr)
    {
        EmitSize emit_size = 0;

        emit_size += Emittable::EmitBytecode(out_arr);

        for (auto &str_inst : string_pull_) {
            size_t str_size = str_inst.first.size();
            emit_size += Emittable::EmitBytecode(out_arr, str_inst.first.c_str(), str_size + 1, str_size);
        }

        return emit_size;
    }

private:
    std::unordered_map<std::string, size_t> string_pull_;
    std::stack<Instruction *> resolution_table_;

    size_t size_ = 0;
    EmitRef offset_ = 0;
}

} // namespace evm::asm2byte

#endif // EVM_ASSEMBLER_ASM_TO_BYTE__STRING_POOL_H
