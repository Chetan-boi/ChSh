#include "executor.hpp"
// #include "lexer.hpp"
#include <string>
#include <filesystem>
#include <vector>
#include <iostream>
#include <optional>

using Command = std::vector<std::string>;
using Output = std::optional<std::string>;

namespace ShellCommands {
  Output cd(const std::string dir,std::string& prevDir) {
    try {
      auto temp = std::filesystem::current_path();
      std::filesystem::current_path(dir);
      prevDir = temp;
      return "Switched Dir";
    }
    catch(const std::exception& e) {
      return e.what();
    }
  }
}

std::optional<std::string> executeCommand(const std::vector<std::string>& command) {
  auto commandType = command[0];
  static std::string prevDir;
  if (commandType == "cd") {

    if (command.size() == 1) {
      if (prevDir.empty()) return "No Prev directory Found";
      return ShellCommands::cd(prevDir,prevDir);
    }

    if (command[1] == "~") return ShellCommands::cd("/Users/chetan",prevDir);

    return ShellCommands::cd(command[1],prevDir);
  }

  else if (commandType == "pwd") {
    return std::filesystem::current_path();
  }

  else if (commandType == "exit") {
    std::cout << "You typed exit\n";
    return std::nullopt;
  }

  else {
    return std::nullopt;
  }
}
