#include "asm2byte.h"

#include "common/str_to_opcode.h"
#include "common/utils/string_operations.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <iostream>

namespace evm::asm2byte {

bool AsmToByte::ParseAsm()
{
    PrepareLinesFromBuffer();

    if (!CreateInstructionsFromLines()) {
        std::cerr << "Error in CreateInstructionsFromLines" << std::endl;
        return false;
    }

    return true;
}

bool AsmToByte::ParseAsmFile(const char *filename)
{
    if (!ReadAsmFile(filename)) {
        return false;
    }

    return ParseAsm();
}

bool AsmToByte::ParseAsmString(const std::string &asm_string)
{
    file_buffer_ = asm_string;
    return ParseAsm();
}

bool AsmToByte::DumpInstructionsToBytes()
{
    for (auto it : instructions_) {
        it->InstrToBytes(&bytecode_);
    }
    return true;
}

bool AsmToByte::DumpBytesInBytecode(const char *filename)
{
    std::ofstream outfile(filename, std::ios::out | std::ios::binary);
    outfile.write(reinterpret_cast<char *>(bytecode_.data()), bytecode_.size());
    return true;
}

bool AsmToByte::ReadAsmFile(const char *filename)
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

void AsmToByte::PrepareLinesFromBuffer()
{
    bool is_string_endline_terminated = (file_buffer_.back() == '\n');

    std::size_t line_start_idx = 0;

    size_t i = 0;
    // TODO(): move this cycle to function
    for (; std::isspace(file_buffer_[i]) != 0; ++i) {
        ++line_start_idx;
    }

    // TODO(Stan1slavssKy): to do better error handling
    for (; i < file_buffer_.size(); ++i) {
        if (std::isspace(file_buffer_[i]) != 0 || file_buffer_[i] == ',') {
            if (file_buffer_[i] == '\n' || file_buffer_[i] == '\0') {
                file_buffer_[i] = '\0';
                lines_.push_back(LineInfo {file_buffer_.substr(line_start_idx, i - line_start_idx + 1)});
                line_start_idx = ++i;
                for (; std::isspace(file_buffer_[i]) != 0; ++i) {
                    ++line_start_idx;
                }
            } else {
                file_buffer_[i] = '\0';
            }
        }
    }

    if (!is_string_endline_terminated) {
        // Push last line
        lines_.push_back(LineInfo {file_buffer_.substr(line_start_idx, i - line_start_idx) + '\0'});
    }
}

bool AsmToByte::CreateInstructionsFromLines()
{
    for (auto &it : lines_) {
        std::vector<std::string> line_args = it.GetArgs();

        Opcode opcode = common::StringToOpcode(line_args[0]);

        switch (opcode) {
            case Opcode::INVALID: {
                std::cerr << "Invalid instruction type" << std::endl;
                return false;
            }
            case Opcode::EXIT:
            case Opcode::RET: {
                instructions_.push_back(new Instruction(opcode));
                break;
            }
            case Opcode::ADD:
            case Opcode::SUB:
            case Opcode::MUL:
            case Opcode::DIV:

            case Opcode::AND:
            case Opcode::OR:
            case Opcode::XOR:

            case Opcode::ADDF:
            case Opcode::SUBF:
            case Opcode::MULF:
            case Opcode::DIVF:

            case Opcode::SLTI:
            case Opcode::SMEI:
            case Opcode::SLTF:
            case Opcode::SMEF:

            case Opcode::EQI:
            case Opcode::NEQI:
            case Opcode::EQF:
            case Opcode::NEQF:

            case Opcode::POWER: {
                auto *instr = new Instruction(opcode);
                instructions_.push_back(instr);
                instr->SetRd(GetRegisterIdxFromString(line_args[1]));
                instr->SetRs1(GetRegisterIdxFromString(line_args[2]));
                instr->SetRs2(GetRegisterIdxFromString(line_args[3]));
                break;
            }
            case Opcode::MOV:
            case Opcode::CONVIF:
            case Opcode::PRINTI:
            case Opcode::PRINTF:
            case Opcode::CONVFI:
            case Opcode::SIN:
            case Opcode::COS:
            case Opcode::JMP:
            case Opcode::JMP_REL: {
                auto *instr = new Instruction(opcode);
                instructions_.push_back(instr);
                instr->SetRd(GetRegisterIdxFromString(line_args[1]));
                instr->SetRs1(GetRegisterIdxFromString(line_args[2]));
                break;
            }
            case Opcode::MOVIF: {
                auto *instr = new Instruction(opcode);
                instructions_.push_back(instr);
                instr->SetRd(GetRegisterIdxFromString(line_args[1]));

                int64_t immediate = 0;

                if (common::IsNumber<int64_t>(line_args[2])) {
                    immediate = std::stol(line_args[2]);
                } else if (common::IsNumber<double>(line_args[2])) {
                    std::cerr << "im here" << std::endl;
                    double double_imm = std::stod(line_args[2]);
                    std::memcpy(&immediate, &double_imm, sizeof(immediate));
                }

                instr->Set64Imm(immediate);
                break;
            }
            case Opcode::JMP_IMM:
            case Opcode::JMP_IF_IMM: {
                auto *instr = new Instruction(opcode);
                instructions_.push_back(instr);

                if (common::IsNumber<double>(line_args[1])) {
                    std::cerr << "Error immediate in jump arg" << std::stol(line_args[1]) << "; Arg should be integer"
                              << std::endl;
                    return false;
                }

                instr->Set32Imm(std::stol(line_args[1]));
                break;
            }
            case Opcode::SCANI:
            case Opcode::SCANF:
            case Opcode::ACCR: {
                auto *instr = new Instruction(opcode);
                instructions_.push_back(instr);
                instr->SetRd(GetRegisterIdxFromString(line_args[1]));
                break;
            }
            case Opcode::CALL: {
                auto *instr = new Instruction(opcode);
                instructions_.push_back(instr);
                break;
            }
            case Opcode::JMP_IF: {
                auto *instr = new Instruction(opcode);
                instructions_.push_back(instr);
                instr->SetRs1(GetRegisterIdxFromString(line_args[1]));
                instr->SetRs2(GetRegisterIdxFromString(line_args[2]));
                break;
            }
            default:
                std::cerr << "Default should not be reachable" << std::endl;
                return false;
        }
    }
    return true;
}

/* static */
int AsmToByte::GetRegisterIdxFromString(const std::string &reg_name)
{
    if (reg_name.front() != 'x') {
        std::cerr << "Invalid register prefix: " << reg_name << "; Only x prefix is support" << std::endl;
        return -1;
    }

    std::string::const_iterator it = reg_name.begin() + 1;
    while (it != reg_name.end() && std::isdigit(*it)) {
        ++it;
    }

    if (it != reg_name.end()) {
        std::cerr << "Invalid register name: " << reg_name << "; Only <x number> format is supported" << std::endl;
        return false;
    }

    return std::stoi(reg_name.substr(1, reg_name.size() - 1));
}

} // namespace evm::asm2byte
