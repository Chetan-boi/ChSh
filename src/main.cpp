#include "lexer.hpp"
#include "executor.hpp"
#include <vector>
#include <iostream>
#include <fstream>

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
  outFile.open("/Users/chetan/.ChSh_history",std::ios::app);
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
