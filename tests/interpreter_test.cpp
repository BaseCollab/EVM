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

TEST_F(InterpreterTest, ADD_UINT1)
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
    std::memcpy(bytecode + 16, &imm2, sizeof(imm1));

    vm_->Execute(bytecode);

    ASSERT_EQ(vm_->GetReg(Reg::X0), imm1 + imm2);
}

TEST_F(InterpreterTest, ADD_UINT_STRESS)
{
    // clang-format off
    //                                                 |-----------------imm_---------------|
    std::vector<byte_t> bytecode = { Opcode::MOVI, Reg::X1, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                     Opcode::MOVI, Reg::X2, 0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    // clang-format on

    uint64_t imm1 = 1;
    uint64_t imm2 = 19;

    std::memcpy(bytecode.data() + 4, &imm1, sizeof(imm1));
    std::memcpy(bytecode.data() + 16, &imm2, sizeof(imm1));

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

} // namespace evm
