#ifndef EVM_ASSEMBLER_ASM_TO_BYTE_ASM_TO_BYTE_H
#define EVM_ASSEMBLER_ASM_TO_BYTE_ASM_TO_BYTE_H

#include "common/macros.h"
#include "file_format/file.h"
#include "line.h"

#include <vector>
#include <string>

namespace evm::asm2byte {

class AsmToByte {
public:
    NO_COPY_SEMANTIC(AsmToByte);
    NO_MOVE_SEMANTIC(AsmToByte);

    AsmToByte() = default;
    ~AsmToByte() = default;

    bool ParseAsmString(const std::string &asm_string, file_format::File *file_arch);
    bool ParseAsmFile(const char *filename, file_format::File *file_arch);

    void SetFileBuffer(const std::string &buffer)
    {
        file_buffer_ = buffer;
    }

private:
    bool ParseAsm(file_format::File *file_arch);
    bool ReadAsmFile(const char *filename);

    void PrepareLinesFromBuffer();
    bool GenRawInstructions(file_format::File *file_arch);

    static int GetRegisterIdxFromString(const std::string &reg_name);

private:
    std::string file_buffer_;

    std::vector<LineInfo> lines_;
};

} // namespace evm::asm2byte

#endif // EVM_ASSEMBLER_ASM_TO_BYTE_ASM_TO_BYTE_H
