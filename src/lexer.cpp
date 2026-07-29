#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>
// #include <optional>
// #include <sstream>

std::ostream& operator<<(std::ostream& out,const std::vector<std::string>& arr) {
  for (auto i : arr) {
    out << i << '\n';
  }
  return out;
}

std::string getCurrentDir() {
  std::string dir = std::filesystem::current_path();
  // std::string rootPath = "/Users/chetan/Desktop/gitgud/Cpp/Projects/Shell";
  const std::string homePath = std::getenv("HOME");

  // if (dir.starts_with(rootPath)) {
    // dir.replace(0,rootPath.length(),"()");
    // return dir;
  // }

  if (dir.starts_with(homePath)) {
    dir.replace(0,homePath.length(),"~");
    return dir;
  }

  return dir;
}

std::vector<std::string> getInput(bool prevCommandStatus) {
  if (prevCommandStatus) std::cout << "\033[31m";
  std::cout << '['
            <<  getCurrentDir()
            << "] >> ";
  if (prevCommandStatus) std::cout << "\033[0m";

  std::string enteredCommand;
  std::getline(std::cin,enteredCommand);

  std::string readWord;
  std::vector<std::string> parsedCommand;
  bool isQuote = false;
  for (std::size_t i{};i<enteredCommand.length()+1;i++) {
    char c = enteredCommand[i];

    // Added ability to identify > and < in ls<cat.txt
    if ((c == '<' || c == '>' || c == '|') && !isQuote) {
      if (!readWord.empty()) {
        parsedCommand.emplace_back(readWord);
        readWord = "";
      }
      if (c == '>' && enteredCommand[i+1] == '>') {
        parsedCommand.emplace_back(">>");
        i++;
      }
      else {
        parsedCommand.emplace_back(std::string(1, c));
      }
      continue;
    }
    
    if ((c == ' ' || c == '\0') && !isQuote) {
      if (!readWord.empty()) {
        parsedCommand.emplace_back(readWord);
        readWord = "";
      }
      continue;
    }
    if (!c) continue;
    if (c == '"') {
      isQuote = !isQuote;
      continue;
    }
    readWord += c; 
  }

  return parsedCommand;
}
