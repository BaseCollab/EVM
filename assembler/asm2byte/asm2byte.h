#ifndef EVM_ASM_2_BYTE_H
#define EVM_ASM_2_BYTE_H

#include "common/macros.h"
#include "common/constants.h"
#include "common/config.h"

#include "instruction.h"
#include "tokenizer.h"

#include "isa/opcodes.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace evm::asm2byte {

class AsmToByte {
public:
    NO_COPY_SEMANTIC(AsmToByte);
    NO_MOVE_SEMANTIC(AsmToByte);

    explicit AsmToByte();
    ~AsmToByte() = default;

    void FillInNameOpcodeMap();

    bool ParseAsmFile(const char *filename);
    bool OpenBytecodeFile(const char *filename);

    Opcode GetOpcodeByName(const std::string &insn_name) const;

private:
    std::string file_buffer_;

    std::vector<byte_t> bytecode_;

    Tokenizer tokenizer_;

    std::unordered_map<std::string, Opcode> name_opcode_map_;

    std::vector<Instruction> lines_info_;
};

} // namespace evm::asm2byte

#endif // EVM_ASM_2_BYTE_H
