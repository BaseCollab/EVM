#include "asm2byte.h"

#include <fstream>
#include <iostream>
#include <algorithm>

namespace evm::asm2byte {

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
