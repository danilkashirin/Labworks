#include <iostream>

struct arguments {
  std::string filename;
  bool list = false;
  std::string frame;
  std::pair<std::string, std::string> rewrite;
  std::string help_frame;
};

arguments FillArguments(int argc, const char** argv);