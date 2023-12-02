#include "asm2byte.h"

#include "common/str_to_opcode.h"
#include "common/utils/string_operations.h"
#include "memory/frame.h"
#include "memory/types/array.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stack>

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
    std::size_t line_start_idx = 0;
    size_t i = 0;

    bool in_line = false;

    do {
        if (in_line == false) {
            if (std::isspace(file_buffer_[i])) {
                continue;
            } else {
                line_start_idx = i;
                in_line = true;
            }
        } else {
            if (std::isspace(file_buffer_[i])) {
                if (file_buffer_[i] == '\n' || file_buffer_[i] == '\0') {
                    file_buffer_[i] = '\0';
                    in_line = false;
                    lines_.push_back(LineInfo {file_buffer_.substr(line_start_idx, i - line_start_idx + 1)});
                } else {
                    file_buffer_[i] = '\0';
                }
            } else if (file_buffer_[i] == ',') {
                file_buffer_[i] = '\0';
            } else if (isalnum(file_buffer_[i]) == 0 && file_buffer_[i] != ':' && file_buffer_[i] != '_' &&
                       file_buffer_[i] != '.' && file_buffer_[i] != '-') {
                std::cerr << "Invalid symbol is assembler file: " << file_buffer_[i] << std::endl;
            }
        }
    } while (++i < file_buffer_.size());

    if (in_line == true) {
        lines_.push_back(LineInfo {file_buffer_.substr(line_start_idx, file_buffer_.size() - 1) + '\0'});
    }
}

bool AsmToByte::CreateInstructionsFromLines()
{
    std::stack<std::pair<std::string, Instruction *>> label_resolving_table;

    size_t offset_cur = 0; // offset in bytecode from the beginning of code section

    for (auto &it : lines_) {
        std::vector<std::string> line_args = it.GetArgs();

        if (line_args[0].back() == ':') {
            labels_.insert({line_args[0].substr(0, line_args[0].size() - 1), offset_cur});
            continue; // no opcode in the line with label:
        }

        Instruction *instr = nullptr;
        Opcode opcode = common::StringToOpcode(line_args[0]);

        if (opcode != Opcode::INVALID) {
            instr = new Instruction(opcode);
            instructions_.push_back(instr);
        }

        // clang-format off
        switch (opcode) {
            case Opcode::INVALID: {
                std::cerr << "Invalid instruction type" << std::endl;
                return false;
            }

            // No arguments

            case Opcode::EXIT:
            case Opcode::RET: {
                break;
            }

            // RD = f(RS1, RS2)

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
                instr->SetRd(GetRegisterIdxFromString(line_args[1]));
                instr->SetRs1(GetRegisterIdxFromString(line_args[2]));
                instr->SetRs2(GetRegisterIdxFromString(line_args[3]));
                break;
            }

            // RS1

            case Opcode::RACC:
            case Opcode::PRINTI:
            case Opcode::PRINTF: {
                instr->SetRs1(GetRegisterIdxFromString(line_args[1]));
                break;
            }

            // RD = f(RS1)

            case Opcode::MOV:

            case Opcode::CONVIF:
            case Opcode::CONVFI:

            case Opcode::SIN:
            case Opcode::COS: {
                instr->SetRd(GetRegisterIdxFromString(line_args[1]));
                instr->SetRs1(GetRegisterIdxFromString(line_args[2]));
                break;
            }

            // RD = f(IMM)

            case Opcode::MOVIF: {
                instr->SetRd(GetRegisterIdxFromString(line_args[1]));

                int64_t immediate = 0;

                if (common::IsNumber<int64_t>(line_args[2])) {
                    immediate = std::stol(line_args[2]);
                } else if (common::IsNumber<double>(line_args[2])) {
                    double double_imm = std::stod(line_args[2]);
                    std::memcpy(&immediate, &double_imm, sizeof(immediate));
                } else {
                    if (labels_.find(line_args[2]) != labels_.end()) {
                        immediate = labels_[line_args[2]];
                    } else {
                        label_resolving_table.push({line_args[2], instr});
                        immediate = 0;
                    }
                }

                instr->Set64Imm(immediate);
                break;
            }

            // RS1, IMM

            case Opcode::JMP_IF_IMM: {
                instr->SetRs1(GetRegisterIdxFromString(line_args[1]));

                int32_t immediate = 0;

                if (common::IsNumber<int32_t>(line_args[2])) {
                    immediate = std::stol(line_args[2]);
                } else if (common::IsNumber<double>(line_args[2])) {
                    std::cerr << "Error immediate in jump arg " << std::stol(line_args[2]) << "; Arg should be integer"
                              << std::endl;
                    return false;
                } else {
                    if (labels_.find(line_args[2]) != labels_.end()) {
                        immediate = labels_[line_args[2]] - offset_cur;
                    } else {
                        label_resolving_table.push({line_args[2], instr});
                        immediate = -offset_cur;
                    }
                }

                instr->Set32Imm(immediate);
                break;
            }

            // RS1, RS2

            case Opcode::JMP_IF: {
                instr->SetRs1(GetRegisterIdxFromString(line_args[1]));
                instr->SetRs2(GetRegisterIdxFromString(line_args[2]));
                break;
            }

            // RD

            case Opcode::SCANI:
            case Opcode::SCANF:
            case Opcode::ACCR: {
                instr->SetRd(GetRegisterIdxFromString(line_args[1]));
                break;
            }

            // IMM

            case Opcode::JMP_IMM: {
                int32_t immediate = 0;

                if (common::IsNumber<int32_t>(line_args[1])) {
                    immediate = std::stol(line_args[1]);
                } else if (common::IsNumber<double>(line_args[1])) {
                    std::cerr << "Error immediate in jump arg " << std::stol(line_args[1]) << "; Arg should be integer"
                              << std::endl;
                    return false;
                } else {
                    if (labels_.find(line_args[1]) != labels_.end()) {
                        immediate = labels_[line_args[1]] - offset_cur;
                    } else {
                        label_resolving_table.push({line_args[1], instr});
                        immediate = -offset_cur;
                    }
                }

                instr->Set32Imm(immediate);
                break;
            }

            // RS1, arguments

            case Opcode::CALL: {
                instr->SetRs1(GetRegisterIdxFromString(line_args[1]));

                size_t n_args = std::min(line_args.size() - 2, Frame::N_PASSED_ARGS_DEFAULT);
                for (size_t i = 0; i < n_args; ++i) {
                    instr->SetArg(i, GetRegisterIdxFromString(line_args[2 + i]));
                }

                break;
            }

            case Opcode::NEWARR: {
                instr->SetRd(GetRegisterIdxFromString(line_args[1]));
                
                auto arr_type = memory::Array::GetTypeFromString(line_args[2]);
                instr->SetRs1(static_cast<byte_t>(arr_type));

                int32_t arr_size = 0;
                if (common::IsNumber<int32_t>(line_args[3])) {
                    arr_size = std::stol(line_args[3]);
                } else if (common::IsNumber<double>(line_args[3])) {
                    std::cerr << "Error immediate in newarr arg " << std::stol(line_args[1]) << "; Arg should be integer"
                              << std::endl;
                    return false;
                }

                instr->Set32Imm(arr_size);
                break;
            }

            case Opcode::LARR: {
                instr->SetRd(GetRegisterIdxFromString(line_args[1]));
                instr->SetRs1(GetRegisterIdxFromString(line_args[2]));
                instr->SetRs2(GetRegisterIdxFromString(line_args[3]));
            }

            case Opcode::STARR: {
                instr->SetRs1(GetRegisterIdxFromString(line_args[1]));
                instr->SetRs2(GetRegisterIdxFromString(line_args[2]));
                // set rs3 aka rd
                instr->SetRd(GetRegisterIdxFromString(line_args[3]));
            }

            default:
                std::cerr << "Default should not be reachable" << std::endl;
                return false;
        }
        // clang-format on

        instr->SetOffset(offset_cur);
        offset_cur += instr->GetBytesSize();
    }

    size_t n_unresolved_labels = 0;

    while (!label_resolving_table.empty()) {
        auto to_resolve = label_resolving_table.top();

        if (labels_.find(to_resolve.first) != labels_.end()) {
            size_t imm_size = to_resolve.second->GetImmSize();
            switch (imm_size) {
                case sizeof(int32_t):
                    to_resolve.second->Add32Imm(labels_[to_resolve.first]);
                    break;
                case sizeof(int64_t):
                    to_resolve.second->Add64Imm(labels_[to_resolve.first]);
                    break;
            }
        } else {
            std::cerr << "Label " << std::quoted(to_resolve.first) << "is unresolved" << std::endl;
            n_unresolved_labels++;
        }

        label_resolving_table.pop();
    }

    if (n_unresolved_labels > 0)
        return false;

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
