#ifndef EVM_ASSEMBLER_ASM_TO_BYTE__HEADER_H
#define EVM_ASSEMBLER_ASM_TO_BYTE__HEADER_H

#include "common/macros.h"
#include "common/constants.h"
#include "emittable.h"
#include "class_section.h"
#include "string_pool.h"

#include <cassert>
#include <vector>
#include <string>

namespace evm::asm2byte {

class Header : Emittable {
public:
    static constexpr EmitMagic MAGIC_NUMBER = 0xEA661221;

public:
    DEFAULT_MOVE_SEMANTIC(Header);
    DEFAULT_COPY_SEMANTIC(Header);

    Header(const std::string name) :
        Emittable(name),
        string_pool_(".string_pool"),
        class_section_(".class")
    {}

    ~Header() = default;

    StringPool *GetStringPool()
    {
        return &string_pool_;
    }

    ClassSection *GetClassSection()
    {
        return &class_section_;
    }

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
        return sizeof(EmitMagic) + 3 * sizeof(EmitRef) +
               string_pool_.GetSize() + class_section_.GetSize();
    }

private:
    StringPool string_pool_;
    ClassSection class_section_;
};

} // namespace evm::asm2byte

#endif // EVM_ASSEMBLER_ASM_TO_BYTE__HEADER_H