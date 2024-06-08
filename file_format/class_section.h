#ifndef EVM_ASSEMBLER_ASM_TO_BYTE__CLASS_SECTION_H
#define EVM_ASSEMBLER_ASM_TO_BYTE__CLASS_SECTION_H

#include "common/macros.h"
#include "common/constants.h"
#include "common/emittable.h"
#include "runtime/memory/type.h"

#include <vector>
#include <cstring>
#include <string>

namespace evm::file_format {

class ClassField : public Emittable {
public:
    using Type = memory::Type;

public:
    DEFAULT_MOVE_SEMANTIC(ClassField);
    DEFAULT_COPY_SEMANTIC(ClassField);

    ClassField(const std::string name = "", Type type = Type::INVALID, hword_t class_idx = -1)
        : Emittable(name), type_(type), class_idx_(class_idx)
    {
    }

    ~ClassField() = default;

    void SetClassRefIdx(hword_t class_idx)
    {
        class_idx_ = class_idx;
    }

    hword_t GetClassRefIdx() const
    {
        return class_idx_;
    }

    size_t GetTypeSize() const
    {
        return static_cast<size_t>(memory::GetSizeOfType(type_));
    }

    void SetType(Type type)
    {
        type_ = type;
    }

    Type GetType() const
    {
        return type_;
    }

    size_t GetRuntimeSize() const
    {
        return GetTypeSize();
    }

    size_t GetSize() const
    {
        return sizeof(Type) + sizeof(EmitClassIdx) + Emittable::GetSize();
    }

    EmitSize EmitBytecode(std::vector<byte_t> *out_arr)
    {
        EmitSize emit_size = 0;

        emit_size += Emittable::EmitBytecode<Type>(out_arr, &type_);
        emit_size += Emittable::EmitBytecode<EmitClassIdx>(out_arr, &class_idx_);
        emit_size += Emittable::EmitBytecode(out_arr);

        return emit_size;
    }

    EmitSize ParseBytecode(const byte_t *in_arr, const EmitSize already_parsed)
    {
        EmitSize parsed_size = already_parsed;

        parsed_size += Emittable::ParseBytecode<Type>(in_arr + parsed_size, &type_);
        parsed_size += Emittable::ParseBytecode<EmitClassIdx>(in_arr + parsed_size, &class_idx_);
        parsed_size += Emittable::ParseBytecode(in_arr, parsed_size);

        return parsed_size - already_parsed;
    }

private:
    Type type_ = Type::INVALID;
    hword_t class_idx_ = -1;
};

class Class : public Section<ClassField> {
public:
    Class(const std::string name = "") : Section<ClassField>(name) {}
    ~Class() = default;

    size_t GetRuntimeSize() const
    {
        size_t runtime_size = 0;
        for (auto &it : instances_) {
            runtime_size += it.GetRuntimeSize();
        }

        return runtime_size;
    }
};

class ClassSection : public Section<Class> {
public:
    ClassSection(const std::string name = "") : Section<Class>(name) {}
    ~ClassSection() = default;

    size_t GetRuntimeSize() const
    {
        return 0; // Class section do nothing in runtime
    }
};

} // namespace evm::file_format

#endif // EVM_ASSEMBLER_ASM_TO_BYTE__CLASS_SECTION_H
