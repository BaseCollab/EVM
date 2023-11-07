#ifndef EVM_ASSEMBLER_ASM_TO_BYTE_ASM_TO_BYTE_H
#define EVM_ASSEMBLER_ASM_TO_BYTE_ASM_TO_BYTE_H

#include "common/macros.h"
#include "common/config.h"

#include "instruction.h"

#include "line.h"

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

namespace evm::asm2byte {

class AsmToByte {
public:
    NO_COPY_SEMANTIC(AsmToByte);
    NO_MOVE_SEMANTIC(AsmToByte);

    AsmToByte() = default;
    ~AsmToByte() = default;

    explicit AsmToByte(const std::string &src)
    {
        ParseAsmString(src);
        DumpInstructionsToBytes();
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

    bool DumpInstructionsToBytes();
    bool DumpBytesInBytecode(const char *filename);

private:
    bool ParseAsm();
    bool ReadAsmFile(const char *filename);

    void PrepareLinesFromBuffer();
    bool CreateInstructionsFromLines();

    static int GetRegisterIdxFromString(const std::string &reg_name);

private:
    std::string file_buffer_;

    std::vector<LineInfo> lines_;

    std::vector<std::unique_ptr<Instruction>> instructions_;

    std::vector<byte_t> bytecode_;

    std::unordered_map<std::string, size_t> labels_;
};

} // namespace evm::asm2byte

#endif // EVM_ASSEMBLER_ASM_TO_BYTE_ASM_TO_BYTE_H
