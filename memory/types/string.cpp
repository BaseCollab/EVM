#include "string.h"
#include "vm/vm.h"
#include "common/crc32.h"

#include <cstring>
#include <iostream>

namespace evm::memory {

/* static */
String *String::Create(const uint8_t *data, size_t length, VirtualMachine *vm)
{
    assert(data != nullptr);
    assert(vm != nullptr);

    auto *allocator = vm->GetAllocator();
    assert(allocator != nullptr);

    void *alloced_ptr = allocator->Alloc(String::GetDataOffset() + length * sizeof(uint8_t));
    
    String *string = reinterpret_cast<String *>(alloced_ptr);
    if (string == nullptr) {
        std::cerr << "Can't create string" << std::endl;
        return nullptr;
    }

    string->SetLength(length);
    string->SetHash(CalculateStringHash(data, length));

    std::memcpy(alloced_ptr + String::GetDataOffset(), data, length);
    
    return string;
}

/* static */
uint32_t CalculateStringHash(const uint8_t *data, size_t length)
{
    return common::Crc32(data, length);
}

} // namespace evm::memory
