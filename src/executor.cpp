#include "executor.hpp"
#include <cstdlib>
#include <string>
#include <filesystem>
#include <sys/wait.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>

using Command = std::vector<std::string>;
using Output = std::string;


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

  int runBinary(const Command& command) {
    pid_t pid = fork();
    if (pid == -1) {
      std::cout << "Command fork failed, returned -1";
      return 1;
    }
    
    if (pid > 0) {
      int status {};
      waitpid(pid, &status, 0);

      if (WIFEXITED(status)) {
        if (WEXITSTATUS(status)) return 1;
        return 0;
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
      std::cout << "Command Failed";
      exit(EXIT_FAILURE);
    }
    return 1;
  }
}

int executeCommand(const std::vector<std::string>& command) {
  if (command.empty()) {
    std::cout << "Please Enter a command";
    return 1;
  }
  
  auto commandType = command[0];
  static std::string prevDir;
  if (commandType == "cd") {
    if (command.size() == 1) {
      if (prevDir.empty()) {
        std::cout << "No Prev directory Found";
        return 1;
      }
      std::cout << ShellCommands::cd(prevDir,prevDir);
      return 0;
    }

    if (command[1] == "~") {
      std::cout << ShellCommands::cd("/Users/chetan",prevDir);
      return 0;
    }

    std::cout << ShellCommands::cd(command[1],prevDir);
    return 0;
  }

  else if (commandType == "pwd") {
    std::cout << std::filesystem::current_path();
    return 0;
  }

  else if (commandType == "exit") {
    std::cout << "You typed exit\nByee!";
    exit(0);
  }

  else {
   return ShellCommands::runBinary(command);
  }
}
