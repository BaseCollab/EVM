#ifndef EVM_ASSEMBLER_ASM_TO_BYTE__EMITTABLE_H
#define EVM_ASSEMBLER_ASM_TO_BYTE__EMITTABLE_H

#include "common/macros.h"

#include <cstring>
#include <string>
#include <vector>

namespace evm::asm2byte {

class Emittable {
public:
    using EmitNameSize = uint8_t;
    using EmitSize     = uint32_t;
    using EmitType     = uint8_t;
    using EmitRef      = EmitSize;
    using EmitMagic    = uint32_t;

public:
    DEFAULT_MOVE_SEMANTIC(Emittable);
    DEFAULT_COPY_SEMANTIC(Emittable);

    Emittable(const std::string &name) :
        name_(name)
    {}

    ~Emittable() = default;

    void SetName(const std::string name)
    {
        name_ = name;
    }

    const std::string &GetName() const
    {
        return name_;
    }

    size_t GetSize() const
    {
        return sizeof(EmitNameSize) + name_.size() + 1;
    }

    template <typename T>
    static T *EmitEmptyBytecode(std::vector<byte_t> *out_arr)
    {
        auto empty_data = out_arr->data() + out_arr->size();
        out_arr->insert(out_arr->end(), sizeof(T), 0);
        return reinterpret_cast<T *>(empty_data);
    }

    static void *EmitEmptyBytecode(std::vector<byte_t> *out_arr, EmitSize insert_size)
    {
        auto empty_data = out_arr->data() + out_arr->size();
        out_arr->insert(out_arr->end(), insert_size, 0);
        return empty_data;
    }

    template <typename T>
    static EmitSize EmitBytecode(std::vector<byte_t> *out_arr, const T *obj)
    {
        T *emitted_empty_data = EmitEmptyBytecode<T>(out_arr);
        memcpy(emitted_empty_data, obj, sizeof(T));

        return sizeof(T);
    }

    static EmitSize EmitBytecode(std::vector<byte_t> *out_arr, const void *obj, EmitSize insert_size, EmitSize cpy_size)
    {
        void *emitted_empty_data = EmitEmptyBytecode(out_arr, insert_size);
        memcpy(emitted_empty_data, obj, cpy_size);

        return insert_size;
    }

    EmitSize EmitBytecode(std::vector<byte_t> *out_arr)
    {
        const EmitNameSize name_size = name_.size() + 1;
        EmitSize emit_size = 0;

        emit_size += EmitBytecode<EmitNameSize>(out_arr, &name_size);
        emit_size += EmitBytecode(out_arr, name_.c_str(), name_size + 1, name_.size());

        return emit_size;
    }

    // size_t ParseBytecode(const std::vector<byte_t> *out_arr)
    // {
    //     // TODO;
    // }

private:
    std::string name_;
};

class Offsetable : public Emittable {
public:
    DEFAULT_MOVE_SEMANTIC(Offsetable);
    DEFAULT_COPY_SEMANTIC(Offsetable);

    Offsetable(const std::string &name, EmitRef offset = 0) :
        Emittable(name),
        offset_(offset)
    {}

    ~Offsetable() = default;

    EmitRef GetOffset() const
    {
        return offset_;
    }

    void SetOffset(EmitRef offset)
    {
        offset_ = offset;
    }

    size_t GetDataOffset() const
    {
        return offset_ + Emittable::GetSize();
    }

private:
    EmitRef offset_ = 0;
};

template <typename T>
class Section : public Offsetable {
public:
    DEFAULT_MOVE_SEMANTIC(Section);
    DEFAULT_COPY_SEMANTIC(Section);

    Section(const std::string name, std::vector<T> instances = {}) :
        Offsetable(name),
        instances_(instances)
    {}

    ~Section() = default;

    std::vector<T> *GetInstances()
    {
        return &instances_;
    }

    void AddInstance(const T &instance)
    {
        instances_.push_back(instance);
    }

    void ClearInstances()
    {
        instances_.clear();
    }

    size_t GetSize() const
    {
        const EmitSize n_instances = instances_.size();
        size_t instances_size = 0;

        for (auto &it : instances_) {
            instances_size += it.GetSize();
        }

        return Emittable::GetSize() + n_instances * sizeof(EmitRef) + sizeof(EmitSize) + instances_size;
    }

    size_t GetDataOffset() const
    {
        const EmitSize n_instances = instances_.size();
        return offset_ + Emittable::GetSize() + n_instances * sizeof(EmitRef) + sizeof(EmitSize);
    }

    EmitSize EmitBytecode(std::vector<byte_t> *out_arr)
    {
        const EmitSize n_instances = instances_.size();
        const EmitSize bytecode_size = out_arr->size();
        std::vector<EmitRef *> instances_starts;

        EmitSize emit_size = 0;

        emit_size += Emittable::EmitBytecode(out_arr);
        emit_size += Emittable::EmitBytecode<EmitSize>(out_arr, &n_instances);

        for (size_t i = 0; i < n_instances; ++i) {
            instances_starts.push_back(Emittable::EmitEmptyBytecode<EmitRef>(out_arr));
            emit_size += sizeof(EmitRef);
        }

        for (size_t i = 0; i < n_instances; ++i) {
            EmitSize instance_size = instances_[i].EmitBytecode(out_arr);
            EmitRef instance_offset = bytecode_size + emit_size;
            memcpy(instances_starts[i], &instance_offset, sizeof(instance_offset));
            emit_size += instance_size;
        }

        return emit_size;
    }

private:
    std::vector<T> instances_;
};

} // namespace evm::asm2byte

#endif // EVM_ASSEMBLER_ASM_TO_BYTE__EMITTABLE_H
