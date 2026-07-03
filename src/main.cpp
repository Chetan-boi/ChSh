#include "lexer.hpp"
#include "executor.hpp"
#include <vector>
#include <iostream>

int main() {
  bool prevCommandStatus = false;
  while(true) {
    std::vector<std::string> parsedCommand = getInput(prevCommandStatus);

    int commandStatus = executeCommand(parsedCommand);

    // Implement Exit in executor

    if (commandStatus == 1) prevCommandStatus = true;
    if (commandStatus == 0) prevCommandStatus = false;

    std::cout << '\n' << std::endl;
  }

  return 0;
}
