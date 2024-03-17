#ifndef EVM_ASSEMBLER_ASM_TO_BYTE__CLASS_SECTION_H
#define EVM_ASSEMBLER_ASM_TO_BYTE__CLASS_SECTION_H

#include "common/macros.h"
#include "common/constants.h"
#include "emittable.h"

#include <vector>
#include <cstring>
#include <string>

namespace evm::asm2byte {

class ClassField : Emittable {
public:
    // clang-format off
    enum Type : Emittable::EmitType {
        INVALID    = 0,
        DOUBLE     = 1,
        BYTE       = 2,
        INT        = 3,
        USER_CLASS = 4,
        ARRAY      = 5, // not used actually right now
        STRING     = 6, // not used actually right now
    };
    // clang-format on

public:
    DEFAULT_MOVE_SEMANTIC(ClassField);
    DEFAULT_COPY_SEMANTIC(ClassField);

    ClassField(const std::string name, Type type, const std::string type_name = "") :
        Emittable(name),
        type_(type),
        type_name_(type_name)
    {}

    ~ClassField() = default;

    void SetTypeName(const std::string name)
    {
        type_name_ = name;
    }

    const std::string &GetTypeName() const
    {
        return type_name_;
    }

    void SetType(Type type)
    {
        type_ = type;
    }

    Type GetType() const
    {
        return type_;
    }

    EmitSize EmitBytecode(std::vector<byte_t> *out_arr)
    {
        EmitNameSize type_name_size = type_name_.size() + 1;
        EmitSize emit_size = 0;

        emit_size += Emittable::EmitBytecode<Type>(out_arr, &type_);
        emit_size += Emittable::EmitBytecode<EmitNameSize>(out_arr, &type_name_size);
        emit_size += Emittable::EmitBytecode(out_arr, type_name_.c_str(), type_name_size + 1, type_name_.size());
        emit_size += Emittable::EmitBytecode(out_arr);

        return emit_size;
    }

    static std::string FieldTypeToString(Type type)
    {
        switch (type) {
            case DOUBLE:
                return "double";
            case BYTE:
                return "byte";
            case INT:
                return "int";
            case USER_CLASS:
                return "class";
            case ARRAY:
                return "array";
            case STRING:
                return "string";
            default:
                return "<invalid>";
        }
    }

    static Type FieldTypeFromString(const std::string &type)
    {
        if (!type.compare("double")) {
            return Type::DOUBLE;
        } else if (!type.compare("byte")) {
            return Type::BYTE;
        } else if (!type.compare("int")) {
            return Type::INT;
        } else if (!type.compare("class")) {
            return Type::USER_CLASS;
        } else if (!type.compare("array")) {
            return Type::ARRAY;
        } else if (!type.compare("string")) {
            return Type::STRING;
        }

        return Type::INVALID;
    }

private:
    Type type_ = Type::INVALID;
    std::string type_name_;
};

using Class = Section<ClassField>;
using ClassSection = Section<Class>;


} // namespace evm::asm2byte

#endif // EVM_ASSEMBLER_ASM_TO_BYTE__CLASS_SECTION_H
