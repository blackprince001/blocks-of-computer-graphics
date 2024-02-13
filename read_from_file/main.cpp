#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

int main()
{
  std::string file_contents;

  {
    std::ifstream file("./input.txt");
    std::stringstream ss;

    if (!file.is_open()) {
      std::cout << "failed to load file";
    }

    ss << file.rdbuf();
    file_contents = ss.str();
  }

  std::cout << file_contents << std::endl;

  return EXIT_SUCCESS;
}