#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
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
  std::string rootPath = "/Users/chetan/Desktop/gitgud/Cpp/Projects/Shell";
  std::string homePath = "/Users/chetan";

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

std::vector<std::string> getInput() {
  std::cout << '['
            <<  getCurrentDir()
            << "] >> ";

  std::string enteredCommand;
  std::getline(std::cin,enteredCommand);

  std::string readWord;
  std::vector<std::string> parsedCommand;
  for (std::size_t i{};i<enteredCommand.length()+1;i++) {
    char c = enteredCommand[i];
    
    if (c == ' ' || c == '\0') {
      parsedCommand.emplace_back(readWord);
      readWord = "";
      continue;
    }
    readWord += c; 
  }

  return parsedCommand;
}
