#ifndef EVM_ASSEMBLER_ASM_TO_BYTE__CODE_SECTION_H
#define EVM_ASSEMBLER_ASM_TO_BYTE__CODE_SECTION_H

#include "common/macros.h"
#include "common/constants.h"
#include "instruction.h"
#include "emittable.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <stack>

namespace evm::asm2byte {

class CodeSection : public Offsetable {
public:
    DEFAULT_MOVE_SEMANTIC(CodeSection);
    DEFAULT_COPY_SEMANTIC(CodeSection);

    CodeSection(const std::string &name, EmitRef offset = 0) :
        Offsetable(name, offset)
    {}

    ~CodeSection() = default;

    size_t GetSize() const
    {
        return size_;
    }

    // ':' at the end of label
    void AddLabel(const std::string &label)
    {
        labels_.insert({label.substr(0, label.size() - 1), size_});
    }

    Instruction *AddInstr(const std::string &name, const Opcode opcode)
    {
        instructions_.push_back({name, opcode});
        return &instructions_.back();
    }

    void ValidateLastInstr()
    {
        instructions_.back().SetOffset(size_);
        size_ += instructions_.back().GetBytesSize();
    }

    void AddInstrToResolve(const std::string &str, Instruction *instr)
    {
        label_resolution_table_.push({str, instr});
    }

    bool ResolveInstrs()
    {
        if (!GetOffset()) {
            return false;
        }

        size_t n_unresolved_labels = 0;

        while (!label_resolution_table_.empty()) {
            auto to_resolve = label_resolution_table_.top();

            if (labels_.find(to_resolve.first) != labels_.end()) {
                size_t imm_size = to_resolve.second->GetImmSize();
                switch (imm_size) {
                    case sizeof(int32_t): // relative values
                        to_resolve.second->Add32Imm(labels_[to_resolve.first] - to_resolve.second->GetOffset());
                        break;
                    case sizeof(int64_t): // absolute values
                        to_resolve.second->Add64Imm(labels_[to_resolve.first] + GetOffset());
                        break;
                }
            } else {
                n_unresolved_labels++;
            }

            label_resolution_table_.pop();
        }

        return n_unresolved_labels == 0;
    }

    EmitSize EmitBytecode(std::vector<byte_t> *out_arr)
    {
        (void)out_arr;
        return 0;
    }

private:

    std::stack<std::pair<std::string, Instruction *>> label_resolution_table_;
    std::unordered_map<std::string, size_t> labels_;

    std::vector<Instruction> instructions_;

    size_t size_ = 0;

};

} // namespace evm::asm2byte

#endif // EVM_ASSEMBLER_ASM_TO_BYTE__CODE_SECTION_H