#include "array.h"
#include "memory/allocator/bump_allocator.h"

#include <iostream>

namespace evm::memory {

void *Array::Create(const VirtualMachine &vm, Type array_type, size_t count)
{
    auto *allocator = vm.GetAllocator();
    assert(allocator != nullptr);

    size_t elem_size = GetSizeOfArrayType(array_type);

    Array *array = allocator->AllocateArray(count * elem_size);
    array->SetSize(count);
    array->SetType(array_type);
    array->SetElemSize(elem_size);
}

void Array::Set(uint64_t value, size_t idx)
{
    switch (type_) {
        case Array::Type::INT:
            std::memcpy(data_ + idx * elem_size_, &value, elem_size_);
            break;
        case Array::Type::DOUBLE:
            std::memcpy(data_ + idx * elem_size_, &value, elem_size_);
            break;
        case Array::Type::INVALID:
            std::cerr << "Set: Array::Type::INVALID" << std::endl;
            break;
        case Array::Type::BOOLEAN:
            std::cerr << "Set: BOOLEAN is unsupported type" << std::endl;
            break;
        case Array::Type::CHAR:
            std::cerr << "Set: CHAR is unsupported type" << std::endl;
            break;
        case Array::Type::BYTE:
            std::cerr << "Set: BYTE is unsupported type" << std::endl;
            break;
        case Array::Type::SHORT:
            std::cerr << "Set: SHORT is unsupported type" << std::endl;
            break;
        case Array::Type::LONG:
            std::cerr << "Set: LONG is unsupported type" << std::endl;
            break;
        default:
            std::cerr << "GetSizeOfArrayType: this line should be unreachable" << std::endl;
            break;
    }
}

void Array::Get(void *value, size_t idx) const
{
    if (idx >= size_) {
        std::cerr << "Invalid idx = " << idx << "; size = " << size_ << std::endl;
        std::exit(EXIT_FAILURE);
    }

    size_t elem_size = GetSizeOfArrayType(type_);
    std::memcpy(value, data_ + idx * elem_size, elem_size);
}

/* static */
size_t GetSizeOfArrayType(Array::Type array_type)
{
    switch (array_type) {
        case Array::Type::INVALID:
            std::cerr << "GetSizeOfArrayType: Array::Type::INVALID" << std::endl;
            return -1;
        case Array::Type::BOOLEAN:
            return 1;
        case Array::Type::CHAR:
            return 1;
        case Array::Type::DOUBLE:
            return 8;
        case Array::Type::BYTE:
            return 1;
        case Array::Type::SHORT:
            return 2;
        case Array::Type::INT:
            return 4;
        case Array::Type::LONG:
            return 8;
        default:
            std::cerr << "GetSizeOfArrayType: this line should be unreachable" << std::endl;
            return -1;
    }
}

/* static */
Array::Type Array::GetTypeFromString(std::string_view string)
{
    if (string == "bool") {
        return Array::Type::BOOLEAN;
    } else if (string == "char") {
        return Array::Type::CHAR;
    } else if (string == "double") {
        return Array::Type::DOUBLE;
    } else if (string == "byte") {
        return Array::Type::BYTE;
    } else if (string == "short") {
        return Array::Type::SHORT;
    } else if (string == "int") {
        return Array::Type::INT;
    } else if (string == "long") {
        return Array::Type::LONG;
    }
    return Array::Type::INVALID;
}

} // namespace evm::memory
