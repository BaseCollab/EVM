#include "array.h"
#include "vm/vm.h"

#include <iostream>
#include <cassert>
#include <cstring>

namespace evm::memory {

/* static */
Array *Array::Create(uint8_t *data, size_t length, Type type, VirtualMachine *vm)
{
    assert(data != nullptr);
    assert(vm != nullptr);

    size_t elem_size = GetSizeOfArrayType(type);
    if (elem_size == 0) {
        std::cerr << "Invalid elem size" << std::endl;
        return nullptr;
    }

    size_t array_size = GetDataOffset() + length * elem_size;
    auto *array = static_cast<Array *>(vm->GetHeapManager()->AllocateObject(array_size));

    if (array == nullptr) {
        std::cerr << "Can't create array" << std::endl;
        return nullptr;
    }

    array->SetLength(length);

    std::memcpy(array->GetData(), data, length * elem_size);
}

/* static */
size_t Array::GetSizeOfArrayType(Array::Type array_type)
{
    switch (array_type) {
        case Array::Type::INVALID:
            std::cerr << "GetSizeOfArrayType: Array::Type::INVALID" << std::endl;
            return 0;
        case Array::Type::DOUBLE:
            return sizeof(uint64_t);
        case Array::Type::BYTE:
            return sizeof(uint8_t);;
        case Array::Type::INT:
            return sizeof(uint64_t);
        default:
            std::cerr << "GetSizeOfArrayType: this line should be unreachable" << std::endl;
            return 0;
    }
}

/* static */
Array::Type Array::GetTypeFromString(std::string_view string)
{
    if (string == "double") {
        return Array::Type::DOUBLE;
    } else if (string == "byte") {
        return Array::Type::BYTE;
    } else if (string == "int") {
        return Array::Type::INT;
    }
    return Array::Type::INVALID;
}

} // namespace evm::memory
