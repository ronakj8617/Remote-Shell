//
// Created by Ronak on 26/05/25.
//
#include <gtest/gtest.h>
#include "CommandExecutor.hpp"

class CommandExecutorTest : public ::testing::Test {
protected:
    CommandExecutor executor;
};

TEST_F(CommandExecutorTest, ExecutesBasicCommand) {
    std::string output = executor.execute("ls");
    ASSERT_FALSE(output.empty());
}

TEST_F(CommandExecutorTest, HandlesInvalidCommand) {
    std::string output = executor.execute("doesnotexistcmd");
    ASSERT_NE(output.find("not found"), std::string::npos);
}

TEST_F(CommandExecutorTest, EchoCommand) {
    std::string output = executor.execute("echo Hello GTest");
    ASSERT_EQ(output, "Hello GTest\n");
}
