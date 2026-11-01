# pragma once
#include <string>
#include <vector>

struct ParsedCommand {
  std::vector<std::string> args;
  std::string inputFile;
  std::string outputFile;
  bool appendMode = false;
};

int executeCommand(std::vector<std::string>&);
