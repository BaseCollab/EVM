#ifndef EVM_ASSEMBLER_ASM_TO_BYTE__HEADER_H
#define EVM_ASSEMBLER_ASM_TO_BYTE__HEADER_H

#include "common/macros.h"
#include "common/constants.h"
#include "common/emittable.h"
#include "class_section.h"
#include "string_pool.h"

#include <iostream>
#include <cassert>
#include <vector>
#include <string>

namespace evm::file_format {

class Header : public Offsetable {
public:
    static constexpr EmitMagic MAGIC_NUMBER = 0xEA661221;
    // String pool, class section and code section
    static constexpr EmitSize N_MAJOR_INSTANCES = 3;

public:
    DEFAULT_MOVE_SEMANTIC(Header);
    DEFAULT_COPY_SEMANTIC(Header);

    Header(const std::string &name) : Offsetable(name), string_pool_(".string_pool"), class_section_(".class") {}
    ~Header() = default;

    StringPool *GetStringPool()
    {
        return &string_pool_;
    }

    ClassSection *GetClassSection()
    {
        return &class_section_;
    }

    size_t GetMajorInstances() const
    {
        return N_MAJOR_INSTANCES;
    }

    EmitSize GetDataOffset() const
    {
        return sizeof(EmitMagic) + GetMajorInstances() * sizeof(EmitRef);
    }

    size_t GetSize() const
    {
        return GetDataOffset() + string_pool_.GetSize() + class_section_.GetSize();
    }

    EmitRef GetStringPoolOffset()
    {
        if (!string_pool_offset_) {
            // Always start data in header with string pool
            string_pool_offset_ = GetDataOffset();
        }
        return string_pool_offset_;
    }

    EmitRef GetClassSectionOffset()
    {
        if (!class_section_offset_) {
            // Always after string pool
            class_section_offset_ = GetStringPoolOffset() + string_pool_.GetSize();
        }
        return class_section_offset_;
    }

    EmitRef GetCodeSectionOffset()
    {
        if (!code_section_offset_) {
            // Always after string pool
            code_section_offset_ = GetClassSectionOffset() + class_section_.GetSize();
        }
        return code_section_offset_;
    }

    EmitSize EmitBytecode(std::vector<byte_t> *out_arr)
    {
        assert(out_arr->size() == 0);
        EmitRef current_offset = 0;

        out_arr->reserve(GetDataOffset() + string_pool_.GetSize() + class_section_.GetSize());

        const EmitMagic magic = MAGIC_NUMBER;
        Emittable::EmitBytecode<EmitMagic>(out_arr, &magic);

        auto *string_pool_offset = Emittable::EmitEmptyBytecode<EmitRef>(out_arr);
        auto *class_section_offset = Emittable::EmitEmptyBytecode<EmitRef>(out_arr);
        auto *code_section_offset = Emittable::EmitEmptyBytecode<EmitRef>(out_arr);

        string_pool_offset_ = current_offset = out_arr->size();
        memcpy(string_pool_offset, &current_offset, sizeof(current_offset));
        string_pool_.EmitBytecode(out_arr);

        class_section_offset_ = current_offset = out_arr->size();
        memcpy(class_section_offset, &current_offset, sizeof(current_offset));
        class_section_.EmitBytecode(out_arr);

        code_section_offset_ = current_offset = out_arr->size();
        memcpy(code_section_offset, &current_offset, sizeof(current_offset));

        return current_offset;
    }

    EmitSize ParseBytecode(const byte_t *in_arr, const EmitSize already_parsed)
    {
        EmitSize parsed_size = already_parsed;
        EmitMagic magic = 0;

        parsed_size += Emittable::ParseBytecode<EmitMagic>(in_arr + parsed_size, &magic);
        if (magic != MAGIC_NUMBER) {
            std::cerr << "Magic number of input file is invalid: " << std::hex << magic << "instead of " << MAGIC_NUMBER
                      << std::dec << std::endl;
            return 0;
        }

        parsed_size += Emittable::ParseBytecode<EmitRef>(in_arr + parsed_size, &string_pool_offset_);
        parsed_size += Emittable::ParseBytecode<EmitRef>(in_arr + parsed_size, &class_section_offset_);
        parsed_size += Emittable::ParseBytecode<EmitRef>(in_arr + parsed_size, &code_section_offset_);

        string_pool_.SetOffset(string_pool_offset_);
        class_section_.SetOffset(class_section_offset_);

        // String pool and code section shouldn't be parsed: no necessaty for it
        parsed_size += class_section_.ParseBytecode(in_arr, class_section_offset_);

        return parsed_size - already_parsed;
    }

private:
    StringPool string_pool_;
    ClassSection class_section_;

    EmitRef string_pool_offset_ = 0;
    EmitRef class_section_offset_ = 0;
    EmitRef code_section_offset_ = 0;
};

} // namespace evm::file_format

#endif // EVM_ASSEMBLER_ASM_TO_BYTE__HEADER_H
