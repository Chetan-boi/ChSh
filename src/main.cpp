#include "lexer.hpp"
#include "executor.hpp"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <csignal>

namespace {
  std::string home = std::getenv("HOME");
}

std::string parsedToString(std::vector<std::string> args) {
  std::string temp;
  for (auto i: args) {
    temp += i;
    temp += ' ';
  }
  return temp;
}

[[noreturn]] int main() {
  bool prevCommandStatus = false;
  std::ofstream outFile;
  outFile.open(home + "/.ChSh_history",std::ios::app);
  signal(SIGINT,SIG_IGN); // Ignore SIGINT
  while(true) {
    std::vector<std::string> parsedCommand = getInput(prevCommandStatus);

    int commandStatus = executeCommand(parsedCommand);

    // Implement Exit in executor

    if (commandStatus == 0) {
      prevCommandStatus = false;
      outFile << parsedToString(parsedCommand) << std::endl;
      outFile.flush();
    }
    else if (commandStatus == 1) {
      prevCommandStatus = true;
    }

    std::cout << '\n' << std::endl;
  }
}
