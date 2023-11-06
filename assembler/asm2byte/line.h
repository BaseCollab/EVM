#ifndef EVM_ASSEMBLER_ASM_TO_BYTE_LINE_H
#define EVM_ASSEMBLER_ASM_TO_BYTE_LINE_H

#include "common/macros.h"
#include "common/constants.h"
#include "common/macros.h"

#include <vector>
#include <string>
#include <string_view>
#include <iostream>
namespace evm::asm2byte {

class LineInfo {
public:
    DEFAULT_MOVE_SEMANTIC(LineInfo);
    DEFAULT_COPY_SEMANTIC(LineInfo);

    LineInfo() = default;
    ~LineInfo() = default;

    explicit LineInfo(std::string line) : line_(line)
    {
        size_t token_start_idx = 0;
        for (size_t i = 0; i < line_.size(); ++i) {
            if (line_[i] == '\0') { // all isspace() symbols are replaced here to '\0'
                args_.push_back(line_.substr(token_start_idx, i - token_start_idx));
                token_start_idx = ++i;
                for (; line_[i] == '\0'; ++i) {
                    ++token_start_idx;
                }
            }
        }
    }

    const std::string &GetLine() const
    {
        return line_;
    }

    const std::vector<std::string> &GetArgs() const
    {
        return args_;
    }

private:
    std::string line_;

    std::vector<std::string> args_;
};

} // namespace evm::asm2byte

#endif // EVM_ASSEMBLER_ASM_TO_BYTE_LINE_H
