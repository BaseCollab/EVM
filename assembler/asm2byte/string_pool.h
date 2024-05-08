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

class StringPool : public Offsetable {
public:
    DEFAULT_MOVE_SEMANTIC(StringPool);
    DEFAULT_COPY_SEMANTIC(StringPool);

    StringPool(const std::string name, EmitRef offset = 0) :
        Offsetable(name, offset)
    {}

    ~StringPool() = default;

    size_t GetSize() const
    {
        return Emittable::GetSize() + size_;
    }

    bool HasInstance(const std::string &str) const
    {
        auto string_pos = string_map_.find(str);
        return string_pos != string_map_.end();
    }

    void AddInstance(const std::string &str)
    {
        string_map_[str] = size_;
        string_pull_.push_back(str);
        size_ += str.size() + 1;
    }

    void AddInstrToResolve(Instruction *instr)
    {
        resolution_table_.push_back(instr);
    }

    bool ResolveInstrs()
    {
        // Not set offset => don't know real offset of string literals => cannot resolve instsrs
        if (!GetOffset()) {
            return false;
        }

        for (auto &to_resolve : resolution_table_) {
            to_resolve->Set32Imm(string_map_[to_resolve->GetStringOp()] + GetDataOffset());
        }

        resolution_table_.clear();

        return true;
    }

    EmitSize EmitBytecode(std::vector<byte_t> *out_arr)
    {
        EmitSize emit_size = Emittable::EmitBytecode(out_arr);

        for (auto &str_inst : string_pull_) {
            size_t str_size = str_inst.size();
            emit_size += Emittable::EmitBytecode(out_arr, str_inst.c_str(), str_size + 1, str_size);
        }

        return emit_size;
    }

    EmitSize ParseBytecode(const byte_t *in_arr, const EmitSize already_parsed)
    {
        // String pool shouldn't be parsed, only emitted
        // So code here is valid
        (void)in_arr;
        (void)already_parsed;

        return 0;
    }

private:
    std::unordered_map<std::string, size_t> string_map_;
    std::vector<std::string> string_pull_;
    std::vector<Instruction *> resolution_table_;

    size_t size_ = 0;
};

} // namespace evm::asm2byte

#endif // EVM_ASSEMBLER_ASM_TO_BYTE__STRING_POOL_H
