#include "executor.hpp"
#include "lexer.hpp"
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <string>
#include <filesystem>
#include <sys/wait.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <fstream>

using Command = std::vector<std::string>;
using Output = std::string;
using Directory = std::string;

std::string command_output;
bool shouldSave = false;
bool wasSuccessfull = false;

namespace ShellCommands {
  Output cd(const Directory dir,Directory& prevDir) {
    try {
      auto temp = std::filesystem::current_path();
      std::filesystem::current_path(dir);
      prevDir = temp;
      return "Switched Dir";
    }
    catch(const std::exception& e) {
      std::cout << e.what() << std::endl;
      return e.what();
    }
  }

  int runBinary(const Command& command) {
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid == -1) {
      std::cout << "Command fork failed, returned -1" << std::endl;
      return 1;
    }
    
    if (pid > 0) {
      int status {};

      if (shouldSave) {
        close(fd[1]);
        char buffer[4096];
        ssize_t bytesRead;

        while((bytesRead = read(fd[0], buffer, sizeof(buffer))) > 0) {
          command_output.append(buffer,bytesRead);
        }

        close(fd[0]);
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
          if (WEXITSTATUS(status)) return 1;
          return 0;
        }
        return 1;
      }
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
      if (shouldSave) {

        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);

        close(fd[1]);
        execvp(args[0],args.data());

        perror("execvp");
        std::cout << "Command Failed" << std::endl;
        exit(EXIT_FAILURE);
     }
      
      execvp(args[0],args.data());

      perror("execvp");
      std::cout << "Command Failed" << std::endl;
      exit(EXIT_FAILURE);
    }
    return 1;
  }

  int writeToFile(std::string FileName) {
    std::ofstream outputFile(FileName);

    if (!outputFile.is_open()) {
      std::cout << "Error opening file" << std::endl;
      return 1;
    }
    outputFile << command_output;
    outputFile.close();
    return 0;
  }
}

int executeCommand(Command& command) {
  command_output = "";
  shouldSave = false;
  
  if (command.empty()) {
    std::cout << "Please Enter a command" << std::endl;
    return 1;
  }

  auto found = std::find_if(command.begin(), command.end(), [](const std::string& s) { return s == "<" || s == ">" || s == "|"; });
  std::string op;
  Command toRight;

  if (found != command.end()) {
    shouldSave = true;
  }

  
  if (found != command.end()) {
    int pos= std::distance(command.begin(),found);
    op = command[pos];
    Command tempToRight(command.begin()+pos+1,command.end());
    Command tempToLeft(command.begin(),command.begin()+pos);
    toRight = tempToRight;
    command = tempToLeft;
  }


  
  auto commandType = command[0];
  static Directory prevDir;

  if (commandType == "cd") {
    if (command.size() == 1) {
      if (prevDir.empty()) {
        std::cout << "No Prev directory Found" << std::endl;
        return 1;
      }
      std::cout << ShellCommands::cd(prevDir,prevDir) << std::endl;
      return 0;
    }

    if (command[1] == "~") {
      std::cout << ShellCommands::cd("/Users/chetan",prevDir) << std::endl;
      return 0;
    }

    if (ShellCommands::cd(command[1],prevDir)!="Switched Dir") return 1;
    return 0;
  }

  else if (commandType == "pwd") {
    if (shouldSave) {
      command_output = std::filesystem::current_path().string() + "\n";
    } else {
      std::cout << std::filesystem::current_path().string() << std::endl;
    }
    wasSuccessfull = true;
  }

  else if (commandType == "exit") {
    std::cout << "You typed exit\nByee!" << std::endl;
    exit(0);
  }

  else {
   wasSuccessfull =  ShellCommands::runBinary(command);
  }

  if (op == ">") {
      return ShellCommands::writeToFile(toRight[0]);
  }

  if (!wasSuccessfull) return 0;
  return 1;
}
