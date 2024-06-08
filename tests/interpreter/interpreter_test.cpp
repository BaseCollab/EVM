#include <gtest/gtest.h>
#include <memory>
#include <cstring>
#include <vector>
#include <cstddef>
#include <cmath>

#include "isa/macros.h"
#include "isa/opcodes.h"
#include "runtime/runtime.h"

#include "assembler/asm2byte/asm2byte.h"

namespace evm {

class InterpreterTest : public testing::Test {
public:
    void SetUp() override
    {
        ASSERT_TRUE(runtime::Runtime::Create());
        runtime_ = runtime::Runtime::GetInstance();
    }

    void TearDown() override
    {
        ASSERT_TRUE(runtime::Runtime::Destroy());
    }

    void ExecuteFromSource(const char *source)
    {
        file_format::File file_arch;
        asm2byte::AsmToByte asm2byte;
        asm2byte.ParseAsmString(source, &file_arch);

        runtime_->Execute(&file_arch);
    }

protected:
    runtime::Runtime *runtime_ {nullptr};
};

// Base integer arithmetic operations

TEST_F(InterpreterTest, ADD_INT64)
{
    auto source = R"(
        movif x1, 1
        movif x2, -10
        add x0, x1, x2
        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), 1 + -10);
}

TEST_F(InterpreterTest, SUB_INT64)
{
    auto source = R"(
        movif x1, -11
        movif x2, -23423
        sub x0, x1, x2
        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), -11 - (-23423));
}

TEST_F(InterpreterTest, MUL_INT64)
{
    auto source = R"(
        movif x1, -11
        movif x2, -23423
        mul x0, x1, x2
        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), -11 * (-23423));
}

TEST_F(InterpreterTest, DIV_INT64)
{
    auto source = R"(
        movif x1, -11
        movif x2, -23423
        div x0, x2, x1
        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), -23423 / (-11));
}

TEST_F(InterpreterTest, AND_INT64)
{
    auto source = R"(
        movif x1, -1234322221
        movif x2, -234656723
        and x0, x1, x2
        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), (-1234322221) & (-234656723));
}

TEST_F(InterpreterTest, OR_INT64)
{
    auto source = R"(
        movif x1, -1234322221
        movif x2, -234656723
        or x0, x1, x2
        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), (-1234322221) | (-234656723));
}

TEST_F(InterpreterTest, XOR_INT64)
{
    auto source = R"(
        movif x1, -1234322221
        movif x2, -234656723
        xor x0, x1, x2
        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), (-1234322221) ^ (-234656723));
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

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetDouble(), 3.14);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), -11);
}

// Comparison integer operations

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

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 1);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 0);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 1);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 0);
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

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 0);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 1);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 0);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 1);
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

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 0);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 1);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 0);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 0);
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

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 1);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 0);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 1);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 1);
}

// Type conversion operations (float64 <-> int64)

TEST_F(InterpreterTest, CONVIF)
{
    auto source = R"(
        movif x1, -123
        convif x2, x1
        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x2)->GetDouble(), -123.0);
}

TEST_F(InterpreterTest, CONVFI)
{
    auto source = R"(
        movif x1, -123.0
        convfi x2, x1
        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x2)->GetInt64(), -123);
}

// Base float64 arithmetic operations

TEST_F(InterpreterTest, ADDF)
{
    auto source = R"(
        movif x1, 1.0
        movif x2, -10.0
        addf x0, x1, x2
        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetDouble(), 1.0 - 10.0);
}

TEST_F(InterpreterTest, SUBF)
{
    auto source = R"(
        movif x1, 1.0
        movif x2, -10.0
        subf x0, x1, x2
        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetDouble(), 1.0 + 10.0);
}

TEST_F(InterpreterTest, MULF)
{
    auto source = R"(
        movif x1, 3.14
        movif x2, -4.12
        mulf x0, x1, x2
        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetDouble(), -3.14 * 4.12);
}

TEST_F(InterpreterTest, DIVF)
{
    auto source = R"(
        movif x1, 3.14
        movif x2, -4.12
        divf x0, x1, x2
        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetDouble(), -3.14 / 4.12);
}

// Comparison float64 operations

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

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 1);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 0);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 1);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 0);
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

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 0);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 1);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 0);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 1);
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

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 0);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 1);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 0);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 0);
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

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 1);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 0);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 1);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x8)->GetInt64(), 1);
}

// Primitive maths operations

TEST_F(InterpreterTest, SIN_COS)
{
    auto source = R"(
        movif x1, 23.0
        movif x2, 32.0
        sin x5, x1
        cos x6, x2

        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetDouble(), std::sin(23.0));
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetDouble(), std::cos(32.0));
}

// Control-flow operations

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

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x2)->GetInt64(), 16);
}

TEST_F(InterpreterTest, RACC_ACCR)
{
    auto source = R"(
        movif x1, 2.0
        racc x1
        accr x5, x1

        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetDouble(), 2.0);
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

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x1)->GetInt64(), 11 + 1);
}

// Array-related operations

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

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x9)->GetInt64(), 3);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetInt64(), 6);
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

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x9)->GetDouble(), 9.0);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x7)->GetDouble(), 7.0);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetDouble(), 3.0);
}

// String-related operations

TEST_F(InterpreterTest, STRING_PULL)
{
    auto source = R"(
        str_immut x0, 'kek lol'
        str_immut x1, 'kek lol'
        str_immut x2, 'kek lol'

        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(),
              runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x1)->GetInt64());
}

// User-objects operations

TEST_F(InterpreterTest, CLASS_SECTION)
{
    auto source = R"(
        .class Foo
            int x;
            double y;
        .class

        .class UU
            class Foo f;
            double i;
        .class

        movif x1, 23
        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x1)->GetInt64(), 23);
}

TEST_F(InterpreterTest, CLASS_SECTION_STRING_PULL)
{
    auto source = R"(
        .class Foo
            int x;
            double y;
        .class

        .class UU
            class Foo f;
            double i;
        .class

        str_immut x0, 'kek lol'
        str_immut x1, 'kek lol'
        str_immut x2, 'kek lol cheburek'

        movif x1, 23
        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x1)->GetInt64(), 23);
}

TEST_F(InterpreterTest, CLASS_OBJECTS)
{
    auto source = R"(
        .class Foo
            int x;
            double y;
        .class

        .class Boom
            class Foo f1;
            double i;
            class Foo f2;
        .class

        movif x1, 23
        movif x2, -11212

        newobj x7, Boom
        obj_set_field x7, Boom@i, x1

        obj_get_field x8, Boom@f2, x7
        obj_set_field x8, Foo@y, x2

        obj_get_field x10, Foo@y, x8
        obj_get_field x11, Boom@i, x7

        exit
    )";

    ExecuteFromSource(source);

    std::cerr << "\n!!!! test is banned temporarily (not implemented some features) !!!!\n\n";

    /// TODO: uncomment after object operations are implemented
    // ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x10)->GetInt64(), 23);
    // ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x11)->GetInt64(), -11212);
}

TEST_F(InterpreterTest, CLASS_ARRAY_OBJECTS)
{
    auto source = R"(
        .class Foo
            int x;
            double y;
        .class

        .class Boom
            class Foo f1;
            double i;

            class Foo f1[10];
            class Foo f2;

            double floats[9999];
            str strings[3];
        .class

        exit
    )";

    ExecuteFromSource(source);

    std::cerr << "\n!!!! test is banned temporarily (not implemented some features) !!!!\n\n";

    /// TODO: uncomment after object operations are implemented
    // ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x10)->GetInt64(), 23);
    // ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x11)->GetInt64(), -11212);
}

TEST_F(InterpreterTest, STRING_COMPARASION)
{
    auto source = R"(
        newstr x0, 'one'
        newstr x1, 'two'
        newstr x2, 'two'
        newstr x3, 'one two'

        print_str x0
        print_str x1
        print_str x3

        strcmp x4, x1, x2
        strcmp x5, x0, x1

        exit
    )";

    ExecuteFromSource(source);

    // Strings shouldn'e be equal even for the same string-literals
    ASSERT_NE(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x1)->GetInt64(),
              runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x2)->GetInt64());

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), 0);
    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 0);
}

TEST_F(InterpreterTest, STRING_CONCAT)
{
    auto source = R"(
        newstr x0, 'one '
        newstr x1, 'two'
        newstr x2, 'one two'

        strconcat x3, x0, x1
        print_str x3
        strcmp x4, x2, x3

        exit
    )";

    ExecuteFromSource(source);

    ASSERT_EQ(runtime_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), 0);
}

} // namespace evm
