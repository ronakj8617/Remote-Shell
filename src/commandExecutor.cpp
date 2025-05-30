//
// Created by Ronak on 30/05/25.
//

#include "CommandExecutor.hpp"
#include <cstdlib>
#include <array>
#include <sstream>
#include <memory>

std::string CommandExecutor::execute(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe) return "popen() failed!";
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
