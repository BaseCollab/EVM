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

TEST_F(InterpreterTest, CONVIF)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 23),
        PUT_R_INSTR(Opcode::CONVIF, 0x2, 0x1),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x2)->GetDouble(), 23.0);
}
TEST_F(InterpreterTest, CONVFI)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 23),
        PUT_R_INSTR(Opcode::CONVIF, 0x2, 0x1),
        PUT_R_INSTR(Opcode::CONVFI, 0x3, 0x2),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x3)->GetInt64(), 23);
}

TEST_F(InterpreterTest, MOV_DOUBLE)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 23),
        PUT_R_INSTR(Opcode::CONVIF, 0x2, 0x1),
        PUT_R_INSTR(Opcode::MOV, 0x4, 0x2),
        PUT_R_INSTR(Opcode::MOV, 0x11, 0x4),
        PUT_R_INSTR(Opcode::CONVFI, 0x12, 0x11),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x12)->GetInt64(), 23);
}

TEST_F(InterpreterTest, ADDF)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 23),
        PUT_R_INSTR(Opcode::CONVIF, 0x2, 0x1),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x3, 32),
        PUT_R_INSTR(Opcode::CONVIF, 0x4, 0x3),
        PUT_A_INSTR(Opcode::ADDF, 0x5, 0x4, 0x2),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetDouble(), 55.0);
}

TEST_F(InterpreterTest, SUBF)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 23),
        PUT_R_INSTR(Opcode::CONVIF, 0x2, 0x1),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x3, 32),
        PUT_R_INSTR(Opcode::CONVIF, 0x4, 0x3),
        PUT_A_INSTR(Opcode::SUBF, 0x5, 0x4, 0x2),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetDouble(), 9.0);
}

TEST_F(InterpreterTest, MULF)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 23),
        PUT_R_INSTR(Opcode::CONVIF, 0x2, 0x1),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x3, 32),
        PUT_R_INSTR(Opcode::CONVIF, 0x4, 0x3),
        PUT_A_INSTR(Opcode::MULF, 0x5, 0x4, 0x2),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetDouble(), 23.0 * 32.0);
}

TEST_F(InterpreterTest, DIVF)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 23),
        PUT_R_INSTR(Opcode::CONVIF, 0x2, 0x1),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x3, 32),
        PUT_R_INSTR(Opcode::CONVIF, 0x4, 0x3),
        PUT_A_INSTR(Opcode::DIVF, 0x5, 0x4, 0x2),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetDouble(), 32.0 / 23.0);
}

TEST_F(InterpreterTest, SLTF)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 23),
        PUT_R_INSTR(Opcode::CONVIF, 0x2, 0x1),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x3, 32),
        PUT_R_INSTR(Opcode::CONVIF, 0x4, 0x3),
        PUT_A_INSTR(Opcode::SLTF, 0x5, 0x4, 0x2),
        PUT_A_INSTR(Opcode::SLTF, 0x6, 0x2, 0x4),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 32.0 < 23.0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 23.0 < 32.0);
}

TEST_F(InterpreterTest, SMEF)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 23),
        PUT_R_INSTR(Opcode::CONVIF, 0x2, 0x1),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x3, 32),
        PUT_R_INSTR(Opcode::CONVIF, 0x4, 0x3),
        PUT_A_INSTR(Opcode::SMEF, 0x5, 0x4, 0x2),
        PUT_A_INSTR(Opcode::SMEF, 0x6, 0x2, 0x4),
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 32.0 >= 23.0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 23.0 >= 32.0);
}

TEST_F(InterpreterTest, EQF)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 23),
        PUT_R_INSTR(Opcode::CONVIF, 0x2, 0x1),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x3, 32),
        PUT_R_INSTR(Opcode::CONVIF, 0x4, 0x3),
        PUT_A_INSTR(Opcode::EQF, 0x5, 0x4, 0x2),

        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 44),
        PUT_R_INSTR(Opcode::CONVIF, 0x2, 0x1),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x3, 44),
        PUT_R_INSTR(Opcode::CONVIF, 0x4, 0x3),
        PUT_A_INSTR(Opcode::EQF, 0x6, 0x4, 0x2),

        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 0);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 1);
}

TEST_F(InterpreterTest, NEQF)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 23),
        PUT_R_INSTR(Opcode::CONVIF, 0x2, 0x1),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x3, 32),
        PUT_R_INSTR(Opcode::CONVIF, 0x4, 0x3),
        PUT_A_INSTR(Opcode::NEQF, 0x5, 0x4, 0x2),

        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 44),
        PUT_R_INSTR(Opcode::CONVIF, 0x2, 0x1),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x3, 44),
        PUT_R_INSTR(Opcode::CONVIF, 0x4, 0x3),
        PUT_A_INSTR(Opcode::NEQF, 0x6, 0x4, 0x2),

        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetInt64(), 1);
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetInt64(), 0);
}

TEST_F(InterpreterTest, SIN_COS)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 23),
        PUT_R_INSTR(Opcode::CONVIF, 0x2, 0x1),
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x3, 32),
        PUT_R_INSTR(Opcode::CONVIF, 0x4, 0x3),

        PUT_R_INSTR(Opcode::SIN, 0x5, 0x2),
        PUT_R_INSTR(Opcode::COS, 0x6, 0x4),

        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x5)->GetDouble(), std::sin(23.0));
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x6)->GetDouble(), std::cos(32.0));
}

TEST_F(InterpreterTest, JMP_IMM)
{
    // clang-format off
    byte_t bytecode[] = {
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x0, 1), // one = 1
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x1, 1), // res = 1
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x2, 2), // two = 2
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x3, 0), // i = 0
        PUT_IMM_INSTR_BYTE(Opcode::MOVIF, 0x4, 5), // number of iters = 5

        // if (i >= 5) goto exit
        PUT_A_INSTR(Opcode::SMEI, 0x5, 0x3, 0x4),
        PUT_INS_INSTR(Opcode::JMP_IF_IMM, 0x5), 0x18, 0x0, 0x0, 0x0, // pc += 24

        PUT_A_INSTR(Opcode::MUL, 0x1, 0x1, 0x2), // res *= 2
        PUT_A_INSTR(Opcode::ADD, 0x3, 0x3, 0x0), // ++i

        PUT_INSTR(Opcode::JMP_IMM), 0xEC, 0xFF, 0xFF, 0xFF, // pc -= 20

        // exit
        PUT_INSTR(Opcode::EXIT)
    };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x1)->GetInt64(), 32);
}

} // namespace evm
