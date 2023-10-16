#ifndef EVM_ASM_2_BYTE_INSTRUCTION_H
#define EVM_ASM_2_BYTE_INSTRUCTION_H

#include "common/macros.h"
#include "common/constants.h"
#include "common/config.h"

#include "isa/opcodes.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace evm::asm2byte {

class Instruction {
public:
    explicit Instruction(std::string line);
    ~Instruction() = default;

    void SetOpcode(Opcode opcode)
    {
        opcode_ = opcode;
    }

    const std::string &GetLine() const
    {
        return line_;
    }

private:
    Opcode opcode_ {Opcode::INVALID};

    reg_t rd {0};
    reg_t rs1 {0};
    reg_t rs2 {0};

    uint64_t imm {0};

    std::string line_;
};

} // namespace evm::asm2byte

#endif // EVM_ASM_2_BYTE_INSTRUCTION_H
