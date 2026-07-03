#include "executor.hpp"
// #include "lexer.hpp"
#include <cstdlib>
#include <string>
#include <filesystem>
#include <sys/wait.h>
#include <vector>
#include <iostream>
#include <optional>
#include <unistd.h>
#include <sys/time.h>

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

  Output runBinary(const Command& command) {
    pid_t pid = fork();
    if (pid == -1) return "Command fork failed, returned -1";
    if (pid > 0) {
      int status {};
      waitpid(pid, &status, 0);

      if (WIFEXITED(status)) {
        return "";
      }
    }
   
    if (pid == 0) {
      std::vector<char*> args;
  
      for (const auto& arg : command) {
        args.emplace_back(const_cast<char*>(arg.c_str()));
      }
  
      args.emplace_back(nullptr);
      execvp(args[0],args.data());

      perror("execvp");
      exit(EXIT_FAILURE);

      return "Command Failed";
    }
    return std::nullopt;
  }
}

std::optional<std::string> executeCommand(const std::vector<std::string>& command) {
  if (command.empty()) return "Please Enter a command";
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
    
    return ShellCommands::runBinary(command);  }
}
