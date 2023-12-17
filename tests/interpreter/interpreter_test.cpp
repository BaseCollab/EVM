#include <gtest/gtest.h>
#include <memory>
#include <cstring>
#include <vector>
#include <cstddef>
#include <cmath>

#include "isa/macros.h"
#include "isa/opcodes.h"
#include "interpreter/interpreter.h"
#include "vm/vm.h"

#include "assembler/asm2byte/asm2byte.h"

namespace evm {

class InterpreterTest : public testing::Test {
public:
    void SetUp() override
    {
        vm_ = std::make_unique<VirtualMachine>();
    }

    void TearDown() override {}

protected:
    std::unique_ptr<VirtualMachine> vm_;
};

TEST_F(InterpreterTest, ADD_INT64)
{
    auto source = R"(
        movif x1, 1
        movif x2, -10
        add x0, x1, x2
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), 1 + -10);
}

TEST_F(InterpreterTest, SUB_INT64)
{
    auto source = R"(
        movif x1, -11
        movif x2, -23423
        sub x0, x1, x2
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), -11 - (-23423));
}

TEST_F(InterpreterTest, MUL_INT64)
{
    auto source = R"(
        movif x1, -11
        movif x2, -23423
        mul x0, x1, x2
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), -11 * (-23423));
}

TEST_F(InterpreterTest, DIV_INT64)
{
    auto source = R"(
        movif x1, -11
        movif x2, -23423
        div x0, x2, x1
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), -23423 / (-11));
}

TEST_F(InterpreterTest, AND_INT64)
{
    auto source = R"(
        movif x1, -1234322221
        movif x2, -234656723
        and x0, x1, x2
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), (-1234322221) & (-234656723));
}

TEST_F(InterpreterTest, OR_INT64)
{
    auto source = R"(
        movif x1, -1234322221
        movif x2, -234656723
        or x0, x1, x2
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), (-1234322221) | (-234656723));
}

TEST_F(InterpreterTest, XOR_INT64)
{
    auto source = R"(
        movif x1, -1234322221
        movif x2, -234656723
        xor x0, x1, x2
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), (-1234322221) ^ (-234656723));
}

TEST_F(InterpreterTest, MOV)
{
    auto source = R"(
        movif x1, 3.14
        movif x2, -11
        mov x3, x1
        mov x4, x2
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetDouble(), 3.14);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), -11);
}

TEST_F(InterpreterTest, SLTI)
{
    auto source = R"(
        movif x1, -123
        movif x2, -11
        movif x3, 23123123
        movif x4, -11

        slti x5, x1, x2
        slti x6, x2, x2
        slti x7, x1, x3
        slti x8, x3, x4

        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 1);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 1);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 0);
}

TEST_F(InterpreterTest, SMEI)
{
    auto source = R"(
        movif x1, -123
        movif x2, -11
        movif x3, 23123123
        movif x4, -11

        smei x5, x1, x2
        smei x6, x2, x2
        smei x7, x1, x3
        smei x8, x3, x4

        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 1);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 1);
}

TEST_F(InterpreterTest, EQ_INT64)
{
    auto source = R"(
        movif x1, -123
        movif x2, -11
        movif x3, 23123123
        movif x4, -11

        eqi x5, x1, x2
        eqi x6, x2, x2
        eqi x7, x1, x3
        eqi x8, x3, x4

        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 1);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 0);
}

TEST_F(InterpreterTest, NEQ_INT64)
{
    auto source = R"(
        movif x1, -123
        movif x2, -11
        movif x3, 23123123
        movif x4, -11

        neqi x5, x1, x2
        neqi x6, x2, x2
        neqi x7, x1, x3
        neqi x8, x3, x4

        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 1);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 1);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 1);
}

TEST_F(InterpreterTest, CONVIF)
{
    auto source = R"(
        movif x1, -123
        convif x2, x1
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x2)->GetDouble(), -123.0);
}

TEST_F(InterpreterTest, CONVFI)
{
    auto source = R"(
        movif x1, -123.0
        convfi x2, x1
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x2)->GetInt64(), -123);
}

TEST_F(InterpreterTest, ADDF)
{
    auto source = R"(
        movif x1, 1.0
        movif x2, -10.0
        addf x0, x1, x2
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetDouble(), 1.0 - 10.0);
}

TEST_F(InterpreterTest, SUBF)
{
    auto source = R"(
        movif x1, 1.0
        movif x2, -10.0
        subf x0, x1, x2
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetDouble(), 1.0 + 10.0);
}

TEST_F(InterpreterTest, MULF)
{
    auto source = R"(
        movif x1, 3.14
        movif x2, -4.12
        mulf x0, x1, x2
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetDouble(), -3.14 * 4.12);
}

TEST_F(InterpreterTest, DIVF)
{
    auto source = R"(
        movif x1, 3.14
        movif x2, -4.12
        divf x0, x1, x2
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetDouble(), -3.14 / 4.12);
}

TEST_F(InterpreterTest, SLTF)
{
    auto source = R"(
        movif x1, -123.2
        movif x2, -11.1
        movif x3, 23123123.1
        movif x4, -11.1

        sltf x5, x1, x2
        sltf x6, x2, x2
        sltf x7, x1, x3
        sltf x8, x3, x4

        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 1);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 1);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 0);
}

TEST_F(InterpreterTest, SMEF)
{
    auto source = R"(
        movif x1, -123.2
        movif x2, -11.1
        movif x3, 23123123.1
        movif x4, -11.1

        smef x5, x1, x2
        smef x6, x2, x2
        smef x7, x1, x3
        smef x8, x3, x4

        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 1);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 1);
}

TEST_F(InterpreterTest, EQF)
{
    auto source = R"(
        movif x1, -123.2
        movif x2, -11.1
        movif x3, 23123123.1
        movif x4, -11.1

        eqf x5, x1, x2
        eqf x6, x2, x2
        eqf x7, x1, x3
        eqf x8, x3, x4

        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 1);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 0);
}

TEST_F(InterpreterTest, NEQF)
{
    auto source = R"(
        movif x1, -123.2
        movif x2, -11.1
        movif x3, 23123123.1
        movif x4, -11.1

        neqf x5, x1, x2
        neqf x6, x2, x2
        neqf x7, x1, x3
        neqf x8, x3, x4

        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 1);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 1);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 1);
}

TEST_F(InterpreterTest, SIN_COS)
{
    auto source = R"(
        movif x1, 23.0
        movif x2, 32.0
        sin x5, x1
        cos x6, x2

        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetDouble(), std::sin(23.0));
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetDouble(), std::cos(32.0));
}

TEST_F(InterpreterTest, JMP_IMM)
{
    auto source = R"(
        movif x1, 2
        movif x2, 1
        movif x3, 10

    loop:
        smei x4, x2, x3
        jmp_if_imm x4, exit

        mul x2, x1, x2

        jmp_imm loop

    exit:
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x2)->GetInt64(), 16);
}

TEST_F(InterpreterTest, RACC_ACCR)
{
    auto source = R"(
        movif x1, 2.0
        racc x1
        accr x5, x1

        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetDouble(), 2.0);
}

TEST_F(InterpreterTest, CALL_RET)
{
    auto source = R"(
        movif x1, 11
        movif x2, inc
        call x2, x1
        accr x1
        jmp_imm exit

    inc:
        movif x5, 1
        add x6, x5, x0
        racc x6
        ret

    exit:
        exit
    )";

    auto asm2byte = asm2byte::AsmToByte(source);
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x1)->GetInt64(), 11 + 1);
}

TEST_F(InterpreterTest, ARRAY_INSTRS_1)
{
    auto source = R"(
        newarr x0, int, 10
        movif x10, 3
        starr x0, x10, x10

        movif x9, 1000

        larr x9, x0, x10
        add x7, x9, x10

        exit
    )";

    auto asm2byte = asm2byte::AsmToByte();
    asm2byte.ParseAsmString(source);
    asm2byte.DumpInstructionsToBytes();
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x9)->GetInt64(), 3);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 6);
}

TEST_F(InterpreterTest, ARRAY_INSTRS_2)
{
    auto source = R"(
        newarr x0, double, 11
        movif x10, 0
        movif x11, 10

    loop:
        movif x2, 1
        add x10, x2, x10

        convif x12, x10
        starr x0, x10, x12

        smei x4, x10, x11
        jmp_if_imm x4, exit
        jmp_imm loop

    exit:
        movif x10, 3
        larr x3, x0, x10

        movif x10, 7
        larr x7, x0, x10

        movif x10, 9
        larr x9, x0, x10

        exit
    )";

    auto asm2byte = asm2byte::AsmToByte();
    asm2byte.ParseAsmString(source);
    asm2byte.DumpInstructionsToBytes();
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x9)->GetDouble(), 9.0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetDouble(), 7.0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetDouble(), 3.0);
}

TEST_F(InterpreterTest, STRING)
{
    auto source = R"(
        string x0, 'kek lol'
        string x1, 'kek lol'
        string x2, 'kek lol'

        exit
    )";

    auto asm2byte = asm2byte::AsmToByte();
    asm2byte.ParseAsmString(source);
    asm2byte.DumpInstructionsToBytes();

    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(),
              vm_->GetInterpreter()->getCurrFrame()->GetReg(0x1)->GetInt64());
}

} // namespace evm
