#include <gtest/gtest.h>

#include "interpreter/interpreter.h"
#include "vm/vm.h"

#include "assembler/asm2byte/asm2byte.h"

namespace evm::memory {

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
        newarray int, 10, x0
        
    )";

    auto asm2byte = asm2byte::AsmToByte();
    asm2byte.ParseAsmString(source);
    asm2byte.DumpInstructionsToBytes();
    std::vector<byte_t> bytecode = asm2byte.GetBytecode();

    vm_->Execute(bytecode.data());
    ASSERT_EQ(vm_->GetInterpreter()->getCurrFrame()->GetReg(0x0)->GetInt64(), 1 + -10);
}

} // namespace evm::memory
