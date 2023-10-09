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

} // namespace evm
