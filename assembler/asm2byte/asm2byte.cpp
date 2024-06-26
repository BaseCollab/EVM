#include "common/logs.h"
#include "common/opcode_to_str.h"
#include "common/str_to_opcode.h"
#include "common/utils/string_operations.h"
#include "file_format/class_section.h"
#include "runtime/memory/type.h"
#include "file_format/file.h"
#include "file_format/header.h"
#include "file_format/code_section.h"
#include "file_format/instruction.h"
#include "asm2byte.h"

#include <cstddef>
#include <cstdlib>
#include <fstream>

namespace evm::asm2byte {

bool AsmToByte::ParseAsm(file_format::File *file_arch)
{
    PrepareLinesFromBuffer();

    if (!GenRawInstructions(file_arch)) {
        PrintErr("Something went wrong");
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
        PrintErr("Filename argument is nullptr");
        return false;
    }

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        PrintErr("Filename ", filename, " didn't open due to some error");
        return false;
    }

    std::streamsize filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    file_buffer_.resize(filesize);

    if (!file.read(file_buffer_.data(), filesize)) {
        PrintErr("Cannot read file into buffer");
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
            } else if (std::isprint(file_buffer_[i]) == 0) {
                PrintErr("Invalid symbol is assembler file: ", file_buffer_[i]);
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
            file_format::ClassField::Type type = memory::GetTypeFromString(line_args[0]);
            auto class_idx = static_cast<ssize_t>(type);
            size_t idx_of_name = 1;

            if (type == file_format::ClassField::Type::CLASS_OBJECT) {
                idx_of_name = 2;

                class_idx = class_section->GetIdxOfInstance(line_args[1]);
                if (class_idx == -1) {
                    PrintErr("Usage of undefined class \"", line_args[1], "\"");
                    return false;
                }
            }

            evm::file_format::ClassField class_field {line_args[idx_of_name]};

            // Now check if it is array
            size_t size_occurance_start = line_args[idx_of_name].find("[");
            size_t size_occurance_end = line_args[idx_of_name].find("]");
            ssize_t array_size = 0;

            if (size_occurance_start != std::string::npos) {
                if (size_occurance_end > size_occurance_start && size_occurance_end != std::string::npos) {
                    array_size = std::stol(line_args[idx_of_name].substr(
                        size_occurance_start + 1, size_occurance_end - size_occurance_start - 1));
                    if (array_size <= 0) {
                        PrintErr("Invalid size of array \"", line_args[idx_of_name], "\"");
                        return false;
                    }

                    class_field.SetName(line_args[idx_of_name].substr(0, size_occurance_start));
                    type = file_format::ClassField::Type::ARRAY_OBJECT;
                } else {
                    PrintErr("Invalid array declaration of class \"", line_args[1], "\"");
                    return false;
                }
            }

            class_field.SetType(type);
            class_field.SetArraySize(array_size);
            class_field.SetClassRefIdx(class_idx);

            class_section->GetInstances()->back().AddInstance(class_field);

            continue;
        }

        // If we here, only assembler instruction is assumed to be in a line
        Opcode opcode = common::StringToOpcode(line_args[0]);
        if (opcode == Opcode::INVALID) {
            PrintErr("Invalid instruction type");
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
            case Opcode::REM:

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

            case Opcode::POWER:
            case Opcode::STRCONCAT:
            case Opcode::STRCMP: {
                instr->SetRd(GetRegisterIdxFromString(line_args[1]));
                instr->SetRs1(GetRegisterIdxFromString(line_args[2]));
                instr->SetRs2(GetRegisterIdxFromString(line_args[3]));
                break;
            }

            // RS1

            case Opcode::RACC:
            case Opcode::PRINTI:
            case Opcode::PRINTF:
            case Opcode::PRINT_STR:
            case Opcode::PRINT_STR_IMMUT: {
                instr->SetRs1(GetRegisterIdxFromString(line_args[1]));
                break;
            }

            // RD = f(RS1)

            case Opcode::MOV:
            case Opcode::CPOBJ:

            case Opcode::CONVIF:
            case Opcode::CONVFI:

            case Opcode::ARR_SIZE:

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
                    code_section->AddInstrToResolve(line_args[2], instr, file_format::CodeSection::ResolutionReason::LABEL_REF);
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
                    PrintErr("Error immediate in jump arg ", std::stol(line_args[2]), "; Arg should be integer");
                    return false;
                } else {
                    code_section->AddInstrToResolve(line_args[2], instr, file_format::CodeSection::ResolutionReason::LABEL_REF);
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
                    PrintErr("Error immediate in jump arg ", std::stol(line_args[1]), "; Arg should be integer");
                    return false;
                } else {
                    code_section->AddInstrToResolve(line_args[1], instr, file_format::CodeSection::ResolutionReason::LABEL_REF);
                }

                instr->Set32Imm(immediate);
                break;
            }

            // RS1, arguments

            case Opcode::CALL: {
                instr->SetRs1(GetRegisterIdxFromString(line_args[1]));

                size_t n_args = std::min(line_args.size() - 2, runtime::Frame::N_PASSED_ARGS_DEFAULT);
                for (size_t i = 0; i < n_args; ++i) {
                    instr->SetArg(i, GetRegisterIdxFromString(line_args[2 + i]));
                }

                break;
            }

            case Opcode::NEWOBJ: {
                instr->SetRd(GetRegisterIdxFromString(line_args[1]));
                code_section->AddInstrToResolve(line_args[2], instr, file_format::CodeSection::ResolutionReason::CLASS_REF);
                break;
            }

            case Opcode::OBJ_SET_FIELD: {
                instr->SetRd(GetRegisterIdxFromString(line_args[3]));
                instr->SetObjRs(GetRegisterIdxFromString(line_args[1]));
                code_section->AddInstrToResolve(line_args[2], instr, file_format::CodeSection::ResolutionReason::CLASS_FIELD_REF);
                break;
            }

            case Opcode::OBJ_GET_FIELD: {
                instr->SetRd(GetRegisterIdxFromString(line_args[1]));
                instr->SetObjRs(GetRegisterIdxFromString(line_args[3]));
                code_section->AddInstrToResolve(line_args[2], instr, file_format::CodeSection::ResolutionReason::CLASS_FIELD_REF);
                break;
            }

            case Opcode::NEWARR_IMM: {
                instr->SetRd(GetRegisterIdxFromString(line_args[1]));
                code_section->AddInstrToResolve(line_args[2], instr, file_format::CodeSection::ResolutionReason::CLASS_REF);

                int32_t arr_size = 0;
                if (common::IsNumber<int32_t>(line_args[3])) {
                    arr_size = std::stol(line_args[3]);
                } else {
                    PrintErr("Error immediate in newarr arg ", std::stol(line_args[1]), "; Arg should be integer");
                    return false;
                }

                instr->Set32Imm(arr_size);
                break;
            }

            case Opcode::NEWARR: {
                instr->SetRd(GetRegisterIdxFromString(line_args[1]));
                code_section->AddInstrToResolve(line_args[2], instr, file_format::CodeSection::ResolutionReason::CLASS_REF);
                instr->SetArrSizeRs(GetRegisterIdxFromString(line_args[3]));
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

            case Opcode::NEWSTR:
            case Opcode::STR_IMMUT: {
                if (!string_pool->HasInstance(line_args[2])) {
                    string_pool->AddInstance(line_args[2]);
                }

                instr->SetRd(GetRegisterIdxFromString(line_args[1]));
                instr->SetStringOp(line_args[2]);
                instr->Set32Imm(0);
                string_pool->AddInstrToResolve(instr);

                break;
            }

            default:
                PrintErr("Cannot handle instruction ", line_args[0]);
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
        PrintErr("Invalid register prefix: ", reg_name, "; Only x prefix is support");
        return -1;
    }

    std::string::const_iterator it = reg_name.begin() + 1;
    while (it != reg_name.end() && std::isdigit(*it)) {
        ++it;
    }

    if (it != reg_name.end()) {
        PrintErr("Invalid register name: ", reg_name, "; Only <x number> format is supported");
        return false;
    }

    return std::stoi(reg_name.substr(1, reg_name.size() - 1));
}

} // namespace evm::asm2byte
