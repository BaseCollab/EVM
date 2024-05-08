#ifndef EVM_ASSEMBLER_ASM_TO_BYTE__FILE_H
#define EVM_ASSEMBLER_ASM_TO_BYTE__FILE_H

#include "common/macros.h"
#include "common/constants.h"
#include "common/emittable.h"
#include "header.h"
#include "code_section.h"

#include <fstream>
#include <vector>

namespace evm::file_format {

class File : public Emittable {
public:
    DEFAULT_MOVE_SEMANTIC(File);
    DEFAULT_COPY_SEMANTIC(File);

    File(const std::string &name) :
        Emittable(name),
        header_(""),
        code_section_(".code")
    {}

    ~File() = default;

    Header *GetHeader()
    {
        return &header_;
    }

    CodeSection *GetCodeSection()
    {
        return &code_section_;
    }

    bool ResolveDependencies()
    {
        if (resolved_dependencies_ == true) {
            return true;
        }

        StringPool *string_pool = header_.GetStringPool();
        string_pool->SetOffset(header_.GetDataOffset());

        if (string_pool->ResolveInstrs() == false) {
            std::cerr << "Couldn't resolve all strings pool references" << std::endl;
            return false;
        }

        ClassSection *class_section = header_.GetClassSection();
        class_section->SetOffset(string_pool->GetOffset() + string_pool->GetSize());

        code_section_.SetOffset(class_section->GetOffset() + class_section->GetSize());
        code_section_.ResolveInstrs();

        resolved_dependencies_ = true;
        return true;
    }

    EmitSize EmitBytecode(std::vector<byte_t> *bytecode)
    {
        return header_.EmitBytecode(bytecode) + code_section_.EmitBytecode(bytecode);
    }

    bool EmitBytecode()
    {
        std::vector<byte_t> bytecode;
        EmitBytecode(&bytecode);

        std::ofstream outfile(GetName(), std::ios::out | std::ios::binary);
        outfile.write(reinterpret_cast<char *>(bytecode.data()), bytecode.size());

        return true;
    }

    EmitSize ParseBytecode(const byte_t *in_arr, const EmitSize already_parsed)
    {
        // Code section shouldn't be parsed, only emitted
        // So code here is valid
        (void)in_arr;
        (void)already_parsed;

        return 0;
    }

private:
    Header header_;
    CodeSection code_section_;

    bool resolved_dependencies_ = false;
};

} // namespace evm::file_format

#endif // EVM_ASSEMBLER_ASM_TO_BYTE__FILE_H
