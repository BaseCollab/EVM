#ifndef EVM_ASSEMBLER_ASM_TO_BYTE__STRING_POOL_H
#define EVM_ASSEMBLER_ASM_TO_BYTE__STRING_POOL_H

#include "common/macros.h"
#include "common/constants.h"
#include "instruction.h"
#include "emittable.h"

#include <cassert>
#include <vector>
#include <cstring>

namespace evm::asm2byte {

class StringPool : Emittable {
public:
    DEFAULT_MOVE_SEMANTIC(StringPool);
    DEFAULT_COPY_SEMANTIC(StringPool);

    StringPool(const std::string name) :
        Emittable(name)
    {}

    ~StringPool() = default;

    EmitSize EmitBytecode(std::vector<byte_t> *out_arr)
    {
        assert(out_arr->size() == 0);
        EmitRef current_offset = 0;

        const EmitMagic magic = MAGIC_NUMBER;
        Emittable::EmitBytecode<EmitMagic>(out_arr, &magic);

        auto *string_pool_offset = Emittable::EmitEmptyBytecode<EmitRef>(out_arr);
        auto *class_section_offset = Emittable::EmitEmptyBytecode<EmitRef>(out_arr);
        auto *code_section_offset = Emittable::EmitEmptyBytecode<EmitRef>(out_arr);

        current_offset = out_arr->size();
        memcpy(string_pool_offset, &current_offset, sizeof(current_offset));
        string_pool_.EmitBytecode(out_arr);

        current_offset = out_arr->size();
        memcpy(class_section_offset, &current_offset, sizeof(current_offset));
        class_section_.EmitBytecode(out_arr);

        current_offset = out_arr->size();
        memcpy(code_section_offset, &current_offset, sizeof(current_offset));

        return current_offset;
    }

    size_t GetSize() const
    {
        // TODO
    }

private:
    std::unordered_map<std::string, size_t> string_pull_;
    std::stack<Instruction *> string_resolving_table;
    size_t string_pull_size = 0;
}

} // namespace evm::asm2byte

#endif // EVM_ASSEMBLER_ASM_TO_BYTE__STRING_POOL_H
