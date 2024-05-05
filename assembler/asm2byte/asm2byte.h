#ifndef EVM_ASSEMBLER_ASM_TO_BYTE_ASM_TO_BYTE_H
#define EVM_ASSEMBLER_ASM_TO_BYTE_ASM_TO_BYTE_H

#include "common/macros.h"
#include "common/config.h"
#include "file_format/code_section.h"
#include "file_format/header.h"
#include "line.h"

#include <vector>
#include <string>

namespace evm::asm2byte {

class AsmToByte {
public:
    static constexpr size_t N_MAX_EMITTED_BYTES = 1 << 17;

public:
    NO_COPY_SEMANTIC(AsmToByte);
    NO_MOVE_SEMANTIC(AsmToByte);

    AsmToByte() :
        header_(""),
        code_section_(".code")
    {
        bytecode_.reserve(N_MAX_EMITTED_BYTES);
    }

    ~AsmToByte() = default;

    AsmToByte(const std::string &src) :
        AsmToByte()
    {
        ParseAsmString(src);
        EmitBytecode();
    }

    bool ParseAsmString(const std::string &asm_string);
    bool ParseAsmFile(const char *filename);

    void SetFileBuffer(const std::string &buffer)
    {
        file_buffer_ = buffer;
    }

    const std::vector<byte_t> &GetBytecode() const
    {
        return bytecode_;
    }

    bool EmitBytecode();
    bool DumpBytesInBytecode(const char *filename);

private:
    bool ParseAsm();
    bool ReadAsmFile(const char *filename);

    void PrepareLinesFromBuffer();
    bool ResolveDependencies(Header *header, CodeSection *code_section);
    bool GenRawInstructions(Header *header, CodeSection *code_section);

    static int GetRegisterIdxFromString(const std::string &reg_name);

private:
    std::string file_buffer_;

    std::vector<LineInfo> lines_;
    std::vector<byte_t> bytecode_;

    Header header_;
    CodeSection code_section_;
};

} // namespace evm::asm2byte

#endif // EVM_ASSEMBLER_ASM_TO_BYTE_ASM_TO_BYTE_H
