#include "array.h"
#include "memory/allocator/bump_allocator.h"

#include <iostream>

namespace evm::memory {

void *Array::Create(const VirtualMachine &vm, Array::Type array_type, size_t count)
{
    auto *allocator = vm.GetAllocator();
    assert(allocator != nullptr);

    size_t elem_size = GetSizeOfArrayType(array_type);

    void *array_ptr = allocator->Alloc(sizeof(Array));
    void *array_data = allocator->Alloc(count * elem_size);

    Array array;
    array.SetSize(count);
    array.SetType(array_type);
    array.SetElemSize(elem_size);
    array.SetData(array_data);

    std::memcpy(array_ptr, &array, sizeof(array));

    return array_ptr;
}

void Array::Set(int64_t value, size_t idx)
{
    switch (type_) {
        case Array::Type::DOUBLE:
        case Array::Type::LONG: {
            std::memcpy(reinterpret_cast<uint8_t *>(data_) + idx * elem_size_, &value, elem_size_);
            break;
        }
        case Array::Type::BYTE: {
            int8_t value_cast = static_cast<int8_t>(value);
            std::memcpy(reinterpret_cast<uint8_t *>(data_) + idx * elem_size_, &value_cast, elem_size_);
            break;
        }
        case Array::Type::SHORT: {
            int16_t value_cast = static_cast<int16_t>(value);
            std::memcpy(reinterpret_cast<uint8_t *>(data_) + idx * elem_size_, &value_cast, elem_size_);
            break;
        }
        case Array::Type::INT: {
            int32_t value_cast = static_cast<int32_t>(value);
            std::memcpy(reinterpret_cast<uint8_t *>(data_) + idx * elem_size_, &value_cast, elem_size_);
            break;
        }
        case Array::Type::INVALID:
            std::cerr << "Set: Array::Type::INVALID" << std::endl;
            break;
        default:
            std::cerr << "GetSizeOfArrayType: this line should be unreachable" << std::endl;
            break;
    }
}

void Array::Get(int64_t *value, size_t idx) const
{
    if (idx >= size_) {
        std::cerr << "Invalid idx = " << idx << "; size = " << size_ << std::endl;
        std::exit(EXIT_FAILURE);
    }

    switch (type_) {
        case Array::Type::DOUBLE:
        case Array::Type::LONG: {
            std::memcpy(value, reinterpret_cast<uint8_t *>(data_) + idx * elem_size_, elem_size_);
            break;
        }
        case Array::Type::BYTE: {
            int8_t value_cast = 0;
            std::memcpy(&value_cast, reinterpret_cast<uint8_t *>(data_) + idx * elem_size_, elem_size_);
            *value = static_cast<int64_t>(value_cast);
            break;
        }
        case Array::Type::SHORT: {
            int16_t value_cast = 0;
            std::memcpy(&value_cast, reinterpret_cast<uint8_t *>(data_) + idx * elem_size_, elem_size_);
            *value = static_cast<int64_t>(value_cast);
            break;
        }
        case Array::Type::INT: {
            int32_t value_cast = 0;
            std::memcpy(&value_cast, reinterpret_cast<uint8_t *>(data_) + idx * elem_size_, elem_size_);
            *value = static_cast<int64_t>(value_cast);
            break;
        }
        case Array::Type::INVALID:
            std::cerr << "Set: Array::Type::INVALID" << std::endl;
            break;
        default:
            std::cerr << "GetSizeOfArrayType: this line should be unreachable" << std::endl;
            break;
    }
}

/* static */
size_t Array::GetSizeOfArrayType(Array::Type array_type)
{
    switch (array_type) {
        case Array::Type::INVALID:
            std::cerr << "GetSizeOfArrayType: Array::Type::INVALID" << std::endl;
            return -1;
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
    if (string == "double") {
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
