#include "lexer.hpp"
#include "executor.hpp"
#include <vector>
#include <iostream>
#include <optional>

int main() {
  while(true) {
    std::vector<std::string> parsedCommand = getInput();

    std::optional<std::string> output = executeCommand(parsedCommand);

    if (!output.has_value()) {
      std::cout << "Bye!";
      return 0;
    }

    std::cout << *output << '\n';

    std::cout << std::endl;
  }
  return 0;
}
