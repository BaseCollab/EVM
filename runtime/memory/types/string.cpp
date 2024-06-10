#include "runtime/runtime.h"
#include "runtime/memory/types/string.h"
#include "common/utils/crc32.h"

#include <cassert>
#include <cstring>
#include <iostream>

namespace evm::runtime::types {

/* static */
String *String::Create(const uint8_t *data, size_t length)
{
    assert(data != nullptr);

    size_t string_size = String::GetDataOffset() + length * sizeof(uint8_t);
    auto *string = static_cast<String *>(Runtime::GetInstance()->GetHeapManager()->AllocateObject(string_size));

    if (string == nullptr) {
        std::cerr << "Can't create string." << std::endl;
        return nullptr;
    }

    string->SetLength(length);
    string->SetHash(String::CalculateStringHash(data, length));

    std::memcpy(reinterpret_cast<uint8_t *>(string) + String::GetDataOffset(), data, length);

    return string;
}

/* static */
uint32_t String::CalculateStringHash(const uint8_t *data, size_t length)
{
    return common::Crc32(data, length);
}

/* static */
int String::CompareStrings(String *str1, String *str2)
{
    if (str1->GetHash() != str2->GetHash()) {
        return false;
    }

    if (str1->GetLength() != str1->GetLength()) {
        return false;
    }

    return std::strcmp(reinterpret_cast<const char *>(str1->GetData()),
                       reinterpret_cast<const char *>(str2->GetData()));
}

/* static */
String *String::ConcatStrings(String *lhs_string, String *rhs_string)
{
    assert(lhs_string != nullptr);
    assert(rhs_string != nullptr);

    size_t lhs_length = lhs_string->GetLength();
    size_t rhs_length = rhs_string->GetLength();
    size_t concat_length = lhs_length + rhs_length;

    uint8_t *concat_data = new uint8_t[concat_length];

    std::memcpy(concat_data, lhs_string->GetData(), lhs_length);
    std::memcpy(concat_data + lhs_length, rhs_string->GetData(), rhs_length);

    auto *concat_string_obj = String::Create(concat_data, concat_length);
    if (UNLIKELY(concat_string_obj == nullptr)) {
        printf("[%s] Can not create concat string from \"%s\" and \"%s\"\n", __func__, lhs_string->GetData(),
               rhs_string->GetData());
        UNREACHABLE();
    }

    delete[] concat_data;

    auto *class_description =
        Runtime::GetInstance()->GetClassManager()->GetDefaultClassDescription(ClassManager::DefaultClassDescr::STRING);
    assert(class_description != nullptr);
    assert(class_description->IsStringObject());

    concat_string_obj->SetClassWord(class_description);

    return concat_string_obj;
}

} // namespace evm::runtime::types
