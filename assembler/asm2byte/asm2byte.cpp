#include "asm2byte.h"

#include <fstream>
#include <iostream>
#include <algorithm>

namespace evm::asm2byte {

AsmToByte::AsmToByte()
{
    FillInNameOpcodeMap();
}

void AsmToByte::FillInNameOpcodeMap()
{
    // clang-format off
    name_opcode_map_.insert({"exit",  Opcode::EXIT});
    name_opcode_map_.insert({"add",   Opcode::ADD});
    name_opcode_map_.insert({"sub",   Opcode::SUB});
    name_opcode_map_.insert({"mul;",  Opcode::MUL});
    name_opcode_map_.insert({"div",   Opcode::DIV});
    name_opcode_map_.insert({"and",   Opcode::AND});
    name_opcode_map_.insert({"or",    Opcode::OR});
    name_opcode_map_.insert({"xor",   Opcode::XOR});
    name_opcode_map_.insert({"mulu",  Opcode::MULU});
    name_opcode_map_.insert({"divu",  Opcode::DIVU});
    name_opcode_map_.insert({"movr",  Opcode::MOVR});
    name_opcode_map_.insert({"movi",  Opcode::MOVI});
    name_opcode_map_.insert({"movf",  Opcode::MOVF});
    name_opcode_map_.insert({"movfr", Opcode::MOVFR});
    name_opcode_map_.insert({"slt",   Opcode::SLT});
    name_opcode_map_.insert({"sltu",  Opcode::SLTU});
    name_opcode_map_.insert({"sme",   Opcode::SME});
    name_opcode_map_.insert({"smeu",  Opcode::SMEU});
    // clang-format on
}

Opcode AsmToByte::GetOpcodeByName(const std::string &insn_name) const
{
    auto it = name_opcode_map_.find(insn_name);
    if (it != name_opcode_map_.end()) {
        return it->second;
    }

    return Opcode::INVALID;
}

bool AsmToByte::ParseAsmFile(const char *filename)
{
    if(!OpenBytecodeFile(filename)) {
        return false;
    }

    tokenizer_.Tokenize(&file_buffer_);

    return true;
}

bool AsmToByte::OpenBytecodeFile(const char *filename)
{
    if (filename == nullptr) {
        std::cerr << "Filename is nullptr" << std::endl;
        return false;
    }

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Error when opening file " << filename << std::endl;
        return false;
    }

    std::streamsize filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    file_buffer_.resize(filesize);

    if (!file.read(file_buffer_.data(), filesize)) {
        std::cerr << "Can not read file in buffer" << std::endl;
        return false;
    }

    file.close();

    return true;
}

} // namespace evm::asm2byte
