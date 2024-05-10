#include "runtime/memory/types/array.h"
#include "runtime/memory/type.h"
#include "runtime/runtime.h"
#include "runtime/memory/allocator/bump_allocator.h"

#include <iostream>

namespace evm::runtime {

void *Array::Create(memory::Type array_type, size_t count)
{
    auto *heap_manager = Runtime::GetInstance()->GetHeapManager();
    assert(heap_manager != nullptr);

    size_t elem_size = GetSizeOfType(array_type);

    void *array_ptr = heap_manager->AllocateObject(sizeof(Array));
    void *array_data = heap_manager->AllocateObject(count * elem_size);

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
        case memory::Type::DOUBLE:
        case memory::Type::INT:
        case memory::Type::OBJECT:
            std::memcpy(reinterpret_cast<uint8_t *>(data_) + idx * elem_size_, &value, elem_size_);
            break;
        case memory::Type::INVALID:
            std::cerr << __func__ << ": invalid array type [" << static_cast<int>(type_) << "] at index [" << idx << "]" << std::endl;
            break;
        default:
            std::cerr << __func__ << ": unsupported array type [" << static_cast<int>(type_) << "] at index [" << idx << "]" << std::endl;
            break;
    }
}

void Array::Get(int64_t *value, size_t idx) const
{
    if (idx >= size_) {
        std::cerr << __func__ << ": invalid idx = " << idx << "; size = " << size_ << std::endl;
        return;
    }

    switch (type_) {
        case memory::Type::DOUBLE:
        case memory::Type::INT:
        case memory::Type::OBJECT:
            std::memcpy(value, reinterpret_cast<uint8_t *>(data_) + idx * elem_size_, elem_size_);
            break;
        case memory::Type::INVALID:
            std::cerr << __func__ << ": invalid array type [" << static_cast<int>(type_) << "] at index [" << idx << "]" << std::endl;
            break;
        default:
            std::cerr << __func__ << ": unsupported array type [" << static_cast<int>(type_) << "] at index [" << idx  << "]" << std::endl;
            break;
    }
}

} // namespace evm::memory
