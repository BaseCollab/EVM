#include <gtest/gtest.h>
#include <memory>
#include <cstring>
#include <vector>
#include <cstddef>

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

TEST_F(InterpreterTest, ADD_UINT)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::ADD, X0, X1, X2,
                          Opcode::EXIT };
    // clang-format on

    uint64_t imm1 = 7;
    uint64_t imm2 = 3;

    std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
    std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 + imm2);
}

TEST_F(InterpreterTest, ADD_SIGNED_INT)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::ADD, X0, X1, X2,
                          Opcode::EXIT };
    // clang-format on

    int64_t imm1 = 1;
    int64_t imm2 = -10;

    std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
    std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 + imm2);
}

TEST_F(InterpreterTest, ADD_UINT_STRESS)
{
    // clang-format off
    //                                                            |-----------------imm_---------------|
    std::vector<byte_t> bytecode = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                     Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    // clang-format on

    uint64_t imm1 = 1;
    uint64_t imm2 = 19;

    std::memcpy(bytecode.data() + 4, &imm1, sizeof(imm1));
    std::memcpy(bytecode.data() + 16, &imm2, sizeof(imm2));

    for (size_t i = 3; i < N_REG; ++i) {
        bytecode.push_back(Opcode::ADD);
        bytecode.push_back(i);
        bytecode.push_back(Reg::X1);
        bytecode.push_back(Reg::X2);
    }

    bytecode.push_back(Opcode::EXIT);

    vm_->Execute(bytecode.data());

    for (size_t i = 3; i < N_REG; ++i) {
        ASSERT_EQ(vm_->GetReg(i), imm1 + imm2);
    }
}

TEST_F(InterpreterTest, SUB_UINT)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::SUB, X0, X1, X2,
                          Opcode::EXIT };
    // clang-format on

    uint64_t imm1 = 11;
    uint64_t imm2 = 2;

    std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
    std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 - imm2);
}

TEST_F(InterpreterTest, SUB_SIGNED_INT)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::SUB, X0, X1, X2,
                          Opcode::EXIT };
    // clang-format on

    int64_t imm1 = -11;
    int64_t imm2 = -22;

    std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
    std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 - imm2);
}

TEST_F(InterpreterTest, SUB_UINT_STRESS)
{
    // clang-format off
    //                                                            |-----------------imm_---------------|
    std::vector<byte_t> bytecode = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                     Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    // clang-format on

    int64_t imm1 = 1;
    int64_t imm2 = 19;

    std::memcpy(bytecode.data() + 4, &imm1, sizeof(imm1));
    std::memcpy(bytecode.data() + 16, &imm2, sizeof(imm2));

    for (size_t i = 3; i < N_REG; ++i) {
        bytecode.push_back(Opcode::SUB);
        bytecode.push_back(i);
        bytecode.push_back(Reg::X1);
        bytecode.push_back(Reg::X2);
    }

    bytecode.push_back(Opcode::EXIT);

    vm_->Execute(bytecode.data());

    for (size_t i = 3; i < N_REG; ++i) {
        ASSERT_EQ(vm_->GetReg(i), imm1 - imm2);
    }
}

TEST_F(InterpreterTest, MUL)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MUL, X0, X1, X2,
                          Opcode::EXIT };
    // clang-format on

    int64_t imm1 = -11;
    int64_t imm2 = -22;

    std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
    std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 * imm2);

    imm1 = -1;
    imm2 = 22;

    std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
    std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 * imm2);

    imm1 = 11;
    imm2 = -2;

    std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
    std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 * imm2);
}

TEST_F(InterpreterTest, MUL_STRESS)
{
    // clang-format off
    //                                                            |-----------------imm_---------------|
    std::vector<byte_t> bytecode = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                     Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    // clang-format on

    int64_t imm1 = 11231;
    int64_t imm2 = -119;

    std::memcpy(bytecode.data() + 4, &imm1, sizeof(imm1));
    std::memcpy(bytecode.data() + 16, &imm2, sizeof(imm2));

    for (size_t i = 3; i < N_REG; ++i) {
        bytecode.push_back(Opcode::MUL);
        bytecode.push_back(i);
        bytecode.push_back(Reg::X1);
        bytecode.push_back(Reg::X2);
    }

    bytecode.push_back(Opcode::EXIT);

    vm_->Execute(bytecode.data());

    for (size_t i = 3; i < N_REG; ++i) {
        ASSERT_EQ(vm_->GetReg(i), imm1 * imm2);
    }
}

TEST_F(InterpreterTest, DIV)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::DIV, X0, X1, X2,
                          Opcode::EXIT };
    // clang-format on

    int64_t imm1 = 121;
    int64_t imm2 = -22;

    std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
    std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 / imm2);

    imm1 = -1;
    imm2 = 22;

    std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
    std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 / imm2);

    imm1 = 1135;
    imm2 = -442;

    std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
    std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 / imm2);
}

TEST_F(InterpreterTest, AND)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::AND, X0, X1, X2,
                          Opcode::EXIT };
    // clang-format on
    {
        int64_t imm1 = 121;
        int64_t imm2 = 12309;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 & imm2);
    }
    {
        int64_t imm1 = -1234;
        int64_t imm2 = 22;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 & imm2);
    }
    {
        uint64_t imm1 = 1135;
        uint64_t imm2 = 12231;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 & imm2);
    }
}

TEST_F(InterpreterTest, OR)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::OR, X0, X1, X2,
                          Opcode::EXIT };
    // clang-format on
    {
        int64_t imm1 = 121;
        int64_t imm2 = 12309;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 | imm2);
    }
    {
        int64_t imm1 = -1234;
        int64_t imm2 = 22;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 | imm2);
    }
    {
        uint64_t imm1 = 1135;
        uint64_t imm2 = 12231;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 | imm2);
    }
}

TEST_F(InterpreterTest, XOR)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::XOR, X0, X1, X2,
                          Opcode::EXIT };
    // clang-format on
    {
        int64_t imm1 = 121;
        int64_t imm2 = 12309;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 ^ imm2);
    }
    {
        int64_t imm1 = -1234;
        int64_t imm2 = 22;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 ^ imm2);
    }
    {
        uint64_t imm1 = 1135;
        uint64_t imm2 = 12231;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 ^ imm2);
    }
}

TEST_F(InterpreterTest, MULU)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MULU, X0, X1, X2,
                          Opcode::EXIT };
    // clang-format on
    {
        uint64_t imm1 = 11;
        uint64_t imm2 = 22;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 * imm2);
    }
    {
        uint64_t imm1 = 144;
        uint64_t imm2 = 222352;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 * imm2);
    }
    {
        uint64_t imm1 = 23411;
        uint64_t imm2 = 24232;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 * imm2);
    }
}

TEST_F(InterpreterTest, DIVU)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::DIVU, X0, X1, X2,
                          Opcode::EXIT };
    // clang-format on
    {
        uint64_t imm1 = 13231;
        uint64_t imm2 = 2232;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 / imm2);
    }
    {
        uint64_t imm1 = 13234;
        uint64_t imm2 = 22;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 / imm2);
    }
    {
        uint64_t imm1 = 23411;
        uint64_t imm2 = 242332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 / imm2);
    }
}

TEST_F(InterpreterTest, MOVR)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVR, Reg::X3, Reg::X1, 0,
                          Opcode::MOVR, Reg::X4, Reg::X2, 0,
                          Opcode::EXIT };
    // clang-format on
    {
        int64_t imm1 = 123;
        int64_t imm2 = 2332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X3), imm1);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2);
    }
    {
        int64_t imm1 = -1323;
        int64_t imm2 = 2332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X3), imm1);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2);
    }
    {
        int64_t imm1 = 1253;
        int64_t imm2 = -2332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X3), imm1);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2);
    }
    {
        uint64_t imm1 = 1253;
        uint64_t imm2 = 26532;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X3), imm1);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2);
    }
}

TEST_F(InterpreterTest, SLT)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::SLT,  Reg::X0, Reg::X1, Reg::X2,
                          Opcode::SLT,  Reg::X4, Reg::X2, Reg::X1,
                          Opcode::EXIT };
    // clang-format on
    {
        int64_t imm1 = -123;
        int64_t imm2 = -2332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 < imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 < imm1);
    }
    {
        int64_t imm1 = 312123;
        int64_t imm2 = -2332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 < imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 < imm1);
    }
    {
        int64_t imm1 = 123123;
        int64_t imm2 = 2332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 < imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 < imm1);
    }
}

TEST_F(InterpreterTest, SLTU)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::SLTU, Reg::X0, Reg::X1, Reg::X2,
                          Opcode::SLTU, Reg::X4, Reg::X2, Reg::X1,
                          Opcode::EXIT };
    // clang-format on
    {
        uint64_t imm1 = 123;
        uint64_t imm2 = 2332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 < imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 < imm1);
    }
    {
        uint64_t imm1 = 312123;
        uint64_t imm2 = 232332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 < imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 < imm1);
    }
    {
        uint64_t imm1 = 123123;
        uint64_t imm2 = 23132;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 < imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 < imm1);
    }
}

TEST_F(InterpreterTest, SME)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::SME,  Reg::X0, Reg::X1, Reg::X2,
                          Opcode::SME,  Reg::X4, Reg::X2, Reg::X1,
                          Opcode::EXIT };
    // clang-format on
    {
        int64_t imm1 = -123;
        int64_t imm2 = -2332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 >= imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 >= imm1);
    }
    {
        int64_t imm1 = 312123;
        int64_t imm2 = -2332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 >= imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 >= imm1);
    }
    {
        int64_t imm1 = 123123;
        int64_t imm2 = 2332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 >= imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 >= imm1);
    }
}

TEST_F(InterpreterTest, SMEU)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::SMEU, Reg::X0, Reg::X1, Reg::X2,
                          Opcode::SMEU, Reg::X4, Reg::X2, Reg::X1,
                          Opcode::EXIT };
    // clang-format on
    {
        uint64_t imm1 = 123;
        uint64_t imm2 = 2332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 >= imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 >= imm1);
    }
    {
        uint64_t imm1 = 312123;
        uint64_t imm2 = 232332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 >= imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 >= imm1);
    }
    {
        uint64_t imm1 = 123123;
        uint64_t imm2 = 23132;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 >= imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 >= imm1);
    }
}

TEST_F(InterpreterTest, EQ)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::EQ,  Reg::X0, Reg::X1, Reg::X2,
                          Opcode::EQ,  Reg::X4, Reg::X2, Reg::X1,
                          Opcode::EXIT };
    // clang-format on
    {
        int64_t imm1 = 123;
        int64_t imm2 = 123;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 == imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 == imm1);
    }
    {
        int64_t imm1 = -312123;
        int64_t imm2 = 2332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 == imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 == imm1);
    }
    {
        int64_t imm1 = -123123;
        int64_t imm2 = -123123;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 == imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 == imm1);
    }
    {
        uint64_t imm1 = 123123;
        uint64_t imm2 = 123123;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 == imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 == imm1);
    }
}

TEST_F(InterpreterTest, NEQ)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                          Opcode::NEQ,  Reg::X0, Reg::X1, Reg::X2,
                          Opcode::NEQ,  Reg::X4, Reg::X2, Reg::X1,
                          Opcode::EXIT };
    // clang-format on
    {
        int64_t imm1 = -123;
        int64_t imm2 = -123;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 != imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 != imm1);
    }
    {
        int64_t imm1 = 312123;
        int64_t imm2 = 312123;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 != imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 != imm1);
    }
    {
        int64_t imm1 = 123123;
        int64_t imm2 = 2332;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 != imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 != imm1);
    }
    {
        uint64_t imm1 = 432;
        uint64_t imm2 = 432;

        std::memcpy(bytecode + 4, &imm1, sizeof(imm1));
        std::memcpy(bytecode + 16, &imm2, sizeof(imm2));

        vm_->Execute(bytecode);

        ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 != imm2);
        ASSERT_EQ(vm_->GetReg(Reg::X4), imm2 != imm1);
    }
}

TEST_F(InterpreterTest, CONVRUF1)
{
    // clang-format off
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X1 = 16U
                          Opcode::CONVRUF, FReg::XF8, Reg::X1, 0,
                          Opcode::EXIT };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetFReg(FReg::XF8), 16.0);
}

TEST_F(InterpreterTest, CONVRSF1)
{
    // clang-format off
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -16
                          Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
                          Opcode::EXIT };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetFReg(FReg::XF8), -16.0);
}

TEST_F(InterpreterTest, CONVFRS1)
{
    // clang-format off
   byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
                          Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
                          Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
                          Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
                          Opcode::SUBF, FReg::XFA, FReg::XF8, FReg::XF9,
                          Opcode::CONVFRS, Reg::X3, FReg::XFA, 0,
                          Opcode::EXIT };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X3), -17);
}

TEST_F(InterpreterTest, CONVFRU1)
{
    // clang-format off
   byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
                          Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
                          Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
                          Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
                          Opcode::ADDF, FReg::XFA, FReg::XF8, FReg::XF9,
                          Opcode::CONVFRU, Reg::X3, FReg::XFA, 0,
                          Opcode::EXIT };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X3), 15);
}

TEST_F(InterpreterTest, MOVF1)
{
    // clang-format off
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
                          Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
                          Opcode::MOVF, FReg::XFA, FReg::XF8, 0,
                          Opcode::MOVF, FReg::XFC, FReg::XFA, 0,
                          Opcode::MOVF, FReg::XF1, FReg::XFC, 0,
                          Opcode::EXIT };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetFReg(FReg::XF1), -1.0);
}

TEST_F(InterpreterTest, ADDF1)
{
    // clang-format off
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
                          Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
                          Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
                          Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
                          Opcode::ADDF, FReg::XFA, FReg::XF9, FReg::XF8,
                          Opcode::EXIT };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetFReg(FReg::XFA), 15.0);
}

TEST_F(InterpreterTest, SUBF1)
{
    // clang-format off
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
                          Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
                          Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
                          Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
                          Opcode::SUBF, FReg::XFA, FReg::XF9, FReg::XF8,
                          Opcode::EXIT };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetFReg(FReg::XFA), 17.0);
}

TEST_F(InterpreterTest, MULF1)
{
    // clang-format off
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -16
                          Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
                          Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
                          Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
                          Opcode::MULF, FReg::XFA, FReg::XF9, FReg::XF8,
                          Opcode::EXIT };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetFReg(FReg::XFA), -256.0);
}

TEST_F(InterpreterTest, DIVF1)
{
    // clang-format off
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -16
                          Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
                          Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
                          Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
                          Opcode::DIVF, FReg::XFA, FReg::XF9, FReg::XF8,
                          Opcode::EXIT };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetFReg(FReg::XFA), -1.0);
}

TEST_F(InterpreterTest, SLTF1)
{
    // clang-format off
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
                          Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
                          Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
                          Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
                          Opcode::SLTF, Reg::X8, FReg::XF8, FReg::XF9,
                          Opcode::SLTF, Reg::X9, FReg::XF9, FReg::XF8,
                          Opcode::EXIT };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X8), 1);
    ASSERT_EQ(vm_->GetReg(Reg::X9), 0);
}

TEST_F(InterpreterTest, SLTF2)
{
    // clang-format off
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
                          Opcode::MOVI, Reg::X2, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X2 = -1
                          Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
                          Opcode::CONVRSF, FReg::XF9, Reg::X2, 0,
                          Opcode::SLTF, Reg::X8, FReg::XF8, FReg::XF9,
                          Opcode::EXIT };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X8), 0);
}

TEST_F(InterpreterTest, SMEF1)
{
    // clang-format off
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
                          Opcode::MOVI, Reg::X2, 0, 0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // X2 = 16U
                          Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
                          Opcode::CONVRUF, FReg::XF9, Reg::X2, 0,
                          Opcode::SMEF, Reg::X8, FReg::XF8, FReg::XF9,
                          Opcode::SMEF, Reg::X9, FReg::XF9, FReg::XF8,
                          Opcode::EXIT };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X8), 0);
    ASSERT_EQ(vm_->GetReg(Reg::X9), 1);
}

TEST_F(InterpreterTest, SMEF2)
{
    // clang-format off
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
                          Opcode::MOVI, Reg::X2, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X2 = -1
                          Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
                          Opcode::CONVRSF, FReg::XF9, Reg::X2, 0,
                          Opcode::SMEF, Reg::X8, FReg::XF8, FReg::XF9,
                          Opcode::EXIT };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X8), 1);
}

TEST_F(InterpreterTest, EQF1)
{
    // clang-format off
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
                          Opcode::MOVI, Reg::X2, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X2 = -1
                          Opcode::MOVI, Reg::X3, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, // X3 != -1
                          Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
                          Opcode::CONVRSF, FReg::XF9, Reg::X2, 0,
                          Opcode::CONVRSF, FReg::XFA, Reg::X3, 0,
                          Opcode::EQF, Reg::X7, FReg::XF8, FReg::XF9,
                          Opcode::EQF, Reg::X8, FReg::XF8, FReg::XFA,
                          Opcode::EXIT };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X7), 1);
    ASSERT_EQ(vm_->GetReg(Reg::X8), 0);
}

TEST_F(InterpreterTest, NEQF1)
{
    // clang-format off
    byte_t bytecode[] = { Opcode::MOVI, Reg::X1, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X1 = -1
                          Opcode::MOVI, Reg::X2, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // X2 = -1
                          Opcode::MOVI, Reg::X3, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, // X3 != -1
                          Opcode::CONVRSF, FReg::XF8, Reg::X1, 0,
                          Opcode::CONVRSF, FReg::XF9, Reg::X2, 0,
                          Opcode::CONVRSF, FReg::XFA, Reg::X3, 0,
                          Opcode::NEQF, Reg::X7, FReg::XF8, FReg::XF9,
                          Opcode::NEQF, Reg::X8, FReg::XF8, FReg::XFA,
                          Opcode::EXIT };
    // clang-format on

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X7), 0);
    ASSERT_EQ(vm_->GetReg(Reg::X8), 1);
}

TEST_F(InterpreterTest, MOVFR1)
{
    // clang-format off
    byte_t bytecode[] = { Opcode::MOVFR, FReg::XF1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // XF1 = 0
                          Opcode::EXIT };
    // clang-format on

    double put_value = 435435.34121346;
    std::memcpy(bytecode + sizeof(insn_size_t), &put_value, sizeof(put_value));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetFReg(FReg::XF1), put_value);
}

} // namespace evm
