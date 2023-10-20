#ifndef EVM_ASM_2_BYTE_TOKENIZER_H
#define EVM_ASM_2_BYTE_TOKENIZER_H

#include "common/macros.h"
#include "common/constants.h"
#include "common/config.h"

#include "isa/generated/opcode.h"
#include "isa/generated/opcode_name_map.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <string_view>
#include <iostream>
#include <cctype>

namespace evm::asm2byte {

class LineInfo {
public:
    DEFAULT_MOVE_SEMANTIC(LineInfo);
    DEFAULT_COPY_SEMANTIC(LineInfo);

    LineInfo() = default;
    ~LineInfo() = default;

    void Parse()
    {
        opcode_ = isa::GetOpcodeByName(tokens_[0]);
        
    }

    void PushToken(std::string_view token)
    {
        tokens_.push_back(std::move(token));
    }

private:
    isa::Opcode opcode_ {isa::Opcode::INVALID};

    std::vector<uint64_t> args_vec_;

    std::vector<std::string_view> tokens_;
};

class Tokenizer {
public:
    explicit Tokenizer() = default;
    ~Tokenizer() = default;

    void TokenizeLines()
    {
        for (auto it: lines_) {
            it.Parse();
        }
    }

    void ParseToLines(std::string &buffer)
    {
        PrepareBuffer(buffer);

        for (size_t line_idx = 0; line_idx < lines_idx_.size(); ++ line_idx) {
            size_t start_idx = (line_idx == 0) ? 0 : lines_idx_[line_idx - 1];
            size_t endl_idx = lines_idx_[line_idx];

            size_t token_start_idx = 0;
            size_t token_end_idx = start_idx;

            LineInfo line_info;

            for (size_t i = start_idx; i < endl_idx; ++i) {
                token_start_idx = buffer.find_first_not_of('\0', token_end_idx);
                if (token_start_idx >= endl_idx) {
                    break;
                }

                token_end_idx = buffer.find_first_of('\0', token_start_idx);
                if (token_end_idx > endl_idx || token_end_idx == std::string::npos) {
                    break;
                }

                line_info.PushToken(std::string_view(buffer.data() + token_start_idx, token_end_idx - token_start_idx));

                i = token_end_idx;
            }
            
            lines_.push_back(std::move(line_info));
        }
    }

    void PrepareBuffer(std::string &buffer)
    {
        for (size_t i = 0; i < buffer.size(); ++i) {
            if (buffer[i] == '\n') {
                lines_idx_.push_back(i);
                buffer[i] = '\0';
            } else if (std::isspace(buffer[i]) != 0 || buffer[i] == ',') {
                buffer[i] = '\0';
            }
        }
    }

    void Tokenize(std::string *buffer)
    {
        ParseToLines(*buffer);
        TokenizeLines();
    }

private:
    std::vector<LineInfo> lines_;
    // index of \n in the .easm file
    std::vector<size_t> lines_idx_;
};

} // namespace evm::asm2byte

#endif // EVM_ASM_2_BYTE_TOKENIZER_H
