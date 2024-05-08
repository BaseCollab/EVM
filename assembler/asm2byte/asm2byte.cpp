#include "common/opcode_to_str.h"
#include "common/str_to_opcode.h"
#include "common/utils/string_operations.h"
#include "runtime/memory/types/array.h"
#include "file_format/file.h"
#include "file_format/header.h"
#include "file_format/code_section.h"
#include "file_format/instruction.h"
#include "asm2byte.h"

#include <fstream>
#include <iostream>

namespace evm::asm2byte {

bool AsmToByte::ParseAsm(file_format::File *file_arch)
{
    PrepareLinesFromBuffer();

    if (!GenRawInstructions(file_arch)) {
        std::cerr << "Error in " << __func__ << std::endl;
        return false;
    }

    return file_arch->ResolveDependencies();
}

bool AsmToByte::ParseAsmFile(const char *filename, file_format::File *file_arch)
{
    if (!ReadAsmFile(filename)) {
        return false;
    }

    return ParseAsm(file_arch);
}

bool AsmToByte::ParseAsmString(const std::string &asm_string, file_format::File *file_arch)
{
    file_buffer_ = asm_string;
    return ParseAsm(file_arch);
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
    bool in_string = false;

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
                    in_string = false;
                    lines_.push_back(LineInfo {file_buffer_.substr(line_start_idx, i - line_start_idx + 1)});
                } else if (in_string != true) {
                    file_buffer_[i] = '\0';
                }
            } else if (file_buffer_[i] == ',' || file_buffer_[i] == ';') {
                file_buffer_[i] = '\0';
            } else if (file_buffer_[i] == '\'') {
                file_buffer_[i] = '\0';

                if (in_string == false)
                    in_string = true;
                else
                    in_string = false;
            } else if (isalnum(file_buffer_[i]) == 0 && file_buffer_[i] != ':' && file_buffer_[i] != '_' &&
                       file_buffer_[i] != '.' && file_buffer_[i] != '-' && file_buffer_[i] != '\'' &&
                       file_buffer_[i] != ';' && file_buffer_[i] != ';' && file_buffer_[i] != '{' &&
                       file_buffer_[i] != '}') {
                std::cerr << "Invalid symbol is assembler file: " << file_buffer_[i] << std::endl;
            }
        }
    } while (++i < file_buffer_.size());

    if (in_line == true) {
        lines_.push_back(LineInfo {file_buffer_.substr(line_start_idx, file_buffer_.size() - 1) + '\0'});
    }
}

bool AsmToByte::GenRawInstructions(file_format::File *file_arch)
{
    file_format::ClassSection *class_section = file_arch->GetHeader()->GetClassSection();
    file_format::StringPool *string_pool = file_arch->GetHeader()->GetStringPool();
    file_format::CodeSection *code_section = file_arch->GetCodeSection();

    bool in_class_defition = false;

    for (auto &it : lines_) {
        std::vector<std::string> line_args = it.GetArgs();

        // Label
        if (line_args[0].back() == ':') {
            code_section->AddLabel(line_args[0]);
            continue;
        }

        // Class definition start/end
        if (!line_args[0].compare(".class")) {
            if (in_class_defition == false) {
                class_section->AddInstance(line_args[1]);
                in_class_defition = true;
            } else {
                in_class_defition = false;
            }

            continue;
        }

        // Class definition fields
        if (in_class_defition == true) {
            file_format::ClassField::Type type = file_format::ClassField::FieldTypeFromString(line_args[0]);

            if (type == file_format::ClassField::Type::USER_CLASS) {
                class_section->GetInstances()->back().AddInstance({line_args[2], type, line_args[1]});
            } else {
                class_section->GetInstances()->back().AddInstance({line_args[1], type});
            }

            continue;
        }

        // If we here, only assembler instruction is assumed to be in a line
        Opcode opcode = common::StringToOpcode(line_args[0]);
        if (opcode == Opcode::INVALID) {
            std::cerr << "Invalid instruction type" << std::endl;
            return false;
        }

        file_format::Instruction *instr = code_section->AddInstr(line_args[0], opcode);

        // clang-format off
        switch (opcode) {
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
            case Opcode::PRINTF:
            case Opcode::PRSTR: {
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
                    code_section->AddInstrToResolve(line_args[2], instr);
                    immediate = 0;
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
                    code_section->AddInstrToResolve(line_args[2], instr);
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
                    code_section->AddInstrToResolve(line_args[1], instr);
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
                break;
            }

            case Opcode::STARR: {
                instr->SetRs1(GetRegisterIdxFromString(line_args[1]));
                instr->SetRs2(GetRegisterIdxFromString(line_args[2]));
                instr->SetRs3(GetRegisterIdxFromString(line_args[3]));
                break;
            }

            case Opcode::STRING: {
                if (!string_pool->HasInstance(line_args[2])) {
                    string_pool->AddInstance(line_args[2]);
                }

                instr->SetRd(GetRegisterIdxFromString(line_args[1]));
                instr->SetStringOp(line_args[2]);

                string_pool->AddInstrToResolve(instr);

                break;
            }

            default:
                std::cerr << "Default should not be reachable" << std::endl;
                return false;
        }
        // clang-format on

        code_section->ValidateLastInstr();
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
