#include <gtest/gtest.h>
#include <memory>
#include <cstring>
#include <vector>
#include <cstddef>

#include "isa/macros.h"
#include "isa/opcodes.h"
#include "interpreter/interpreter.h"
#include "vm/vm.h"

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
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 0),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x2, 0),
        PUT_A_INSTR(Opcode::ADD, 0x0, 0x1, 0x2),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    int64_t imm1 = 1;
    int64_t imm2 = -10;

    std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
    std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 + imm2);
}

TEST_F(InterpreterTest, SUB_INT64)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 0),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x2, 0),
        PUT_A_INSTR(Opcode::SUB, 0x0, 0x1, 0x2),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    int64_t imm1 = -11;
    int64_t imm2 = -22;

    std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
    std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 - imm2);
}

TEST_F(InterpreterTest, MUL_INT64)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 0),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x2, 0),
        PUT_A_INSTR(Opcode::MUL, 0x0, 0x1, 0x2),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    int64_t imm1 = -11;
    int64_t imm2 = -22;

    std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
    std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 * imm2);

    imm1 = -1;
    imm2 = 22;

    std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
    std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 * imm2);

    imm1 = 11;
    imm2 = -2;

    std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
    std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 * imm2);
}

TEST_F(InterpreterTest, MUL_INT64_STRESS)
{
    // clang-format off
    std::vector<byte_t> bytecode = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 0),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x2, 0)
    };
    // clang-format on

    int64_t imm1 = 11231;
    int64_t imm2 = -119;

    std::memcpy(bytecode.data() + sizeof(instr_size_t), &imm1, sizeof(imm1));
    std::memcpy(bytecode.data() + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

    size_t n_regs = 16;

    for (size_t i = 3; i < n_regs; ++i) {
        bytecode.push_back(Opcode::MUL);
        bytecode.push_back(i);
        bytecode.push_back(0x1);
        bytecode.push_back(0x2);
    }

    bytecode.push_back(Opcode::EXIT);
    bytecode.push_back(0x0);
    bytecode.push_back(0x0);
    bytecode.push_back(0x0);

    vm_->Execute(bytecode.data());

    for (size_t i = 3; i < n_regs; ++i) {
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(i)->GetInt64(), imm1 * imm2);
    }
}

TEST_F(InterpreterTest, DIV_INT64)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 0),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x2, 0),
        PUT_A_INSTR(Opcode::DIV, 0x0, 0x1, 0x2),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    int64_t imm1 = 121;
    int64_t imm2 = -22;

    std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
    std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));


    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 / imm2);

    imm1 = -1;
    imm2 = 22;

    std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
    std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 / imm2);

    imm1 = 1135;
    imm2 = 442;

    std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
    std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 / imm2);
}

TEST_F(InterpreterTest, AND_INT64)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 0),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x2, 0),
        PUT_A_INSTR(Opcode::AND, 0x0, 0x1, 0x2),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    {
        int64_t imm1 = 121;
        int64_t imm2 = 12309;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 & imm2);
    }
    {
        int64_t imm1 = -1234;
        int64_t imm2 = 22;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 & imm2);
    }
    {
        uint64_t imm1 = 1135;
        uint64_t imm2 = 12231;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 & imm2);
    }
}

TEST_F(InterpreterTest, OR_INT64)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 0),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x2, 0),
        PUT_A_INSTR(Opcode::OR, 0x0, 0x1, 0x2),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    {
        int64_t imm1 = 12334277;
        int64_t imm2 = 9992563459;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 | imm2);
    }
    {
        int64_t imm1 = -12230532734;
        int64_t imm2 = 2767332;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 | imm2);
    }
    {
        uint64_t imm1 = 11111135;
        uint64_t imm2 = 12217888831;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 | imm2);
    }
}

TEST_F(InterpreterTest, XOR_INT64)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 0),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x2, 0),
        PUT_A_INSTR(Opcode::XOR, 0x0, 0x1, 0x2),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

   {
        int64_t imm1 = 12334277;
        int64_t imm2 = 9992563459;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 ^ imm2);
    }
    {
        int64_t imm1 = -12230532734;
        int64_t imm2 = 2767332;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 ^ imm2);
    }
    {
        uint64_t imm1 = 11111135;
        uint64_t imm2 = 12217888831;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), imm1 ^ imm2);
    }
}

TEST_F(InterpreterTest, MOV_INT64)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 0),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x2, 0),
        PUT_R_INSTR(Opcode::MOV, 0x3, 0x1),
        PUT_R_INSTR(Opcode::MOV, 0x4, 0x2),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    {
        int64_t imm1 = 123;
        int64_t imm2 = 2332;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm2);
    }
    {
        int64_t imm1 = -1323;
        int64_t imm2 = 2332;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm2);
    }
    {
        int64_t imm1 = 1253;
        int64_t imm2 = -2332;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm2);
    }
    {
        uint64_t imm1 = 1253;
        uint64_t imm2 = 26532;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm2);
    }
}

TEST_F(InterpreterTest, SLTI)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 0),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x2, 0),
        PUT_A_INSTR(Opcode::SLTI, 0x3, 0x1, 0x2),
        PUT_A_INSTR(Opcode::SLTI, 0x4, 0x2, 0x1),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    {
        int64_t imm1 = -123;
        int64_t imm2 = -2332;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1 < imm2);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm2 < imm1);
    }
    {
        int64_t imm1 = 312123;
        int64_t imm2 = -2332;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1 < imm2);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm2 < imm1);
    }
    {
        int64_t imm1 = 123123;
        int64_t imm2 = 2332;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1 < imm2);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm2 < imm1);
    }
}

TEST_F(InterpreterTest, SMEI)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 0),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x2, 0),
        PUT_A_INSTR(Opcode::SMEI, 0x3, 0x1, 0x2),
        PUT_A_INSTR(Opcode::SMEI, 0x4, 0x2, 0x1),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    {
        int64_t imm1 = -123;
        int64_t imm2 = -2332;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1 >= imm2);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm2 >= imm1);
    }
    {
        int64_t imm1 = 312123;
        int64_t imm2 = -2332;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1 >= imm2);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm2 >= imm1);
    }
    {
        int64_t imm1 = 2323232323;
        int64_t imm2 = 2323232323;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1 >= imm2);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm2 >= imm1);
    }
}

TEST_F(InterpreterTest, EQ_INT64)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 0),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x2, 0),
        PUT_A_INSTR(Opcode::EQI, 0x3, 0x1, 0x2),
        PUT_A_INSTR(Opcode::EQI, 0x4, 0x2, 0x1),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    {
        int64_t imm1 = 123;
        int64_t imm2 = 123;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1 == imm2);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm1 == imm2);
    }
    {
        int64_t imm1 = -312123;
        int64_t imm2 = 2332;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1 == imm2);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm1 == imm2);
    }
    {
        int64_t imm1 = -123123;
        int64_t imm2 = -123123;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1 == imm2);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm1 == imm2);
    }
    {
        uint64_t imm1 = 123123;
        uint64_t imm2 = 123123;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1 == imm2);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm1 == imm2);
    }
}

TEST_F(InterpreterTest, NEQ_INT64)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 0),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x2, 0),
        PUT_A_INSTR(Opcode::NEQI, 0x3, 0x1, 0x2),
        PUT_A_INSTR(Opcode::NEQI, 0x4, 0x2, 0x1),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    {
        int64_t imm1 = -123;
        int64_t imm2 = -123;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1 != imm2);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm1 != imm2);
    }
    {
        int64_t imm1 = 312123;
        int64_t imm2 = 312123;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1 != imm2);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm1 != imm2);
    }
    {
        int64_t imm1 = 123123;
        int64_t imm2 = 2332;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1 != imm2);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm1 != imm2);
    }
    {
        uint64_t imm1 = 432;
        uint64_t imm2 = 432;

        std::memcpy(bytecode + sizeof(instr_size_t), &imm1, sizeof(imm1));
        std::memcpy(bytecode + sizeof(instr_size_t) * 2 + sizeof(int64_t), &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), imm1 != imm2);
        ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x4)->GetInt64(), imm1 != imm2);
    }
}

// TEST_F(InterpreterTest, CONVRUF1)
// {
//     // clang-format off
//     byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X1 = 16U
//                           Opcode::CONVRUF, FReg::XF8, Reg::X1, 0,
//                           Opcode::EXIT };
//     // clang-format on

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetFReg(FReg::XF8), 16.0);
// }

// TEST_F(InterpreterTest, CONVRSF1)
// {
//     // clang-format off
//     byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -16
//                           Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
//                           Opcode::EXIT };
//     // clang-format on

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetFReg(FReg::XF8), -16.0);
// }

// TEST_F(InterpreterTest, CONVFRS1)
// {
//     // clang-format off
//    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
//                           Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
//                           Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
//                           Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
//                           Opcode::SUBF, FReg::XFA, FReg::XF8, FReg::XF9,
//                           Opcode::CONVFRS, Reg::X3, FReg::XFA, 0,
//                           Opcode::EXIT };
//     // clang-format on

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetReg(Reg::X3), -17);
// }

// TEST_F(InterpreterTest, CONVFRU1)
// {
//     // clang-format off
//    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
//                           Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
//                           Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
//                           Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
//                           Opcode::ADDF, FReg::XFA, FReg::XF8, FReg::XF9,
//                           Opcode::CONVFRU, Reg::X3, FReg::XFA, 0,
//                           Opcode::EXIT };
//     // clang-format on

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetReg(Reg::X3), 15);
// }

// TEST_F(InterpreterTest, MOVF1)
// {
//     // clang-format off
//     byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
//                           Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
//                           Opcode::MOVF, FReg::XFA, FReg::XF8, 0,
//                           Opcode::MOVF, FReg::XFC, FReg::XFA, 0,
//                           Opcode::MOVF, FReg::XF1, FReg::XFC, 0,
//                           Opcode::EXIT };
//     // clang-format on

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetFReg(FReg::XF1), -1.0);
// }

// TEST_F(InterpreterTest, ADDF1)
// {
//     // clang-format off
//     byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
//                           Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
//                           Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
//                           Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
//                           Opcode::ADDF, FReg::XFA, FReg::XF9, FReg::XF8,
//                           Opcode::EXIT };
//     // clang-format on

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetFReg(FReg::XFA), 15.0);
// }

// TEST_F(InterpreterTest, SUBF1)
// {
//     // clang-format off
//     byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
//                           Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
//                           Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
//                           Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
//                           Opcode::SUBF, FReg::XFA, FReg::XF9, FReg::XF8,
//                           Opcode::EXIT };
//     // clang-format on

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetFReg(FReg::XFA), 17.0);
// }

// TEST_F(InterpreterTest, MULF1)
// {
//     // clang-format off
//     byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -16
//                           Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
//                           Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
//                           Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
//                           Opcode::MULF, FReg::XFA, FReg::XF9, FReg::XF8,
//                           Opcode::EXIT };
//     // clang-format on

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetFReg(FReg::XFA), -256.0);
// }

// TEST_F(InterpreterTest, DIVF1)
// {
//     // clang-format off
//     byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -16
//                           Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
//                           Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
//                           Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
//                           Opcode::DIVF, FReg::XFA, FReg::XF9, FReg::XF8,
//                           Opcode::EXIT };
//     // clang-format on

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetFReg(FReg::XFA), -1.0);
// }

// TEST_F(InterpreterTest, SLTF1)
// {
//     // clang-format off
//     byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
//                           Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
//                           Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
//                           Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
//                           Opcode::SLTF, Reg::X8, FReg::XF8, FReg::XF9,
//                           Opcode::SLTF, Reg::X9, FReg::XF9, FReg::XF8,
//                           Opcode::EXIT };
//     // clang-format on

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetReg(Reg::X8), 1);
//     ASSERT_EQ(vm_->GetReg(Reg::X9), 0);
// }

// TEST_F(InterpreterTest, SLTF2)
// {
//     // clang-format off
//     byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
//                           Opcode::MOVI, Reg::X2, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X2 = -1
//                           Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
//                           Opcode::CONVRSF, FReg::XF9, Reg::X2, 0,
//                           Opcode::SLTF, Reg::X8, FReg::XF8, FReg::XF9,
//                           Opcode::EXIT };
//     // clang-format on

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetReg(Reg::X8), 0);
// }

// TEST_F(InterpreterTest, SMEF1)
// {
//     // clang-format off
//     byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
//                           Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
//                           Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
//                           Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
//                           Opcode::SMEF, Reg::X8, FReg::XF8, FReg::XF9,
//                           Opcode::SMEF, Reg::X9, FReg::XF9, FReg::XF8,
//                           Opcode::EXIT };
//     // clang-format on

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetReg(Reg::X8), 0);
//     ASSERT_EQ(vm_->GetReg(Reg::X9), 1);
// }

// TEST_F(InterpreterTest, SMEF2)
// {
//     // clang-format off
//     byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
//                           Opcode::MOVI, Reg::X2, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X2 = -1
//                           Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
//                           Opcode::CONVRSF, FReg::XF9, Reg::X2, 0,
//                           Opcode::SMEF, Reg::X8, FReg::XF8, FReg::XF9,
//                           Opcode::EXIT };
//     // clang-format on

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetReg(Reg::X8), 1);
// }

// TEST_F(InterpreterTest, EQF1)
// {
//     // clang-format off
//     byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
//                           Opcode::MOVI, Reg::X2, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X2 = -1
//                           Opcode::MOVI, Reg::X3, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, // X3 != -1
//                           Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
//                           Opcode::CONVRSF, FReg::XF9, Reg::X2, 0,
//                           Opcode::CONVRSF, FReg::XFA, Reg::X3, 0,
//                           Opcode::EQF, Reg::X7, FReg::XF8, FReg::XF9,
//                           Opcode::EQF, Reg::X8, FReg::XF8, FReg::XFA,
//                           Opcode::EXIT };
//     // clang-format on

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetReg(Reg::X7), 1);
//     ASSERT_EQ(vm_->GetReg(Reg::X8), 0);
// }

// TEST_F(InterpreterTest, NEQF1)
// {
//     // clang-format off
//     byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
//                           Opcode::MOVI, Reg::X2, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X2 = -1
//                           Opcode::MOVI, Reg::X3, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, // X3 != -1
//                           Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
//                           Opcode::CONVRSF, FReg::XF9, Reg::X2, 0,
//                           Opcode::CONVRSF, FReg::XFA, Reg::X3, 0,
//                           Opcode::NEQF, Reg::X7, FReg::XF8, FReg::XF9,
//                           Opcode::NEQF, Reg::X8, FReg::XF8, FReg::XFA,
//                           Opcode::EXIT };
//     // clang-format on

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetReg(Reg::X7), 0);
//     ASSERT_EQ(vm_->GetReg(Reg::X8), 1);
// }

// TEST_F(InterpreterTest, MOVFR1)
// {
//     // clang-format off
//     byte_t bytecode[] = { Opcode::MOVFR, FReg::XF1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // XF1 = 0
//                           Opcode::EXIT };
//     // clang-format on

//     double put_value = 435435.34121346;
//     std::memcpy(bytecode + sizeof(insn_size_t), &put_value, sizeof(put_value));

//     vm_->Execute(bytecode);

//     ASSERT_EQ(vm_->GetFReg(FReg::XF1), put_value);
// }

} // namespace evm
