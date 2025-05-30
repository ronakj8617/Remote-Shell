//
// Created by Ronak on 26/05/25.
//
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "CommandExecutor.hpp"


TEST_CASE("CommandExecutor executes simple commands", "[command]") {
    CommandExecutor executor;

    SECTION("Executes ls without error") {
        std::string output = executor.execute("ls");
        REQUIRE_FALSE(output.empty());
    }

    SECTION("Handles unknown commands gracefully") {
        std::string output = executor.execute("ls1");
        REQUIRE(((int)output.find("not found") != std::string::npos));
    }

    SECTION("Handles multi-word commands") {
        std::string output = executor.execute("echo Hello World");
        REQUIRE(output == "Hello World\n");
    }
}
