#pragma once
#include <string>

struct arguments {
  int16_t length = 0;
  uint16_t width = 0;
  std::string input;
  std::string output;
  uint64_t max_iter = 0;
  uint64_t freq = 0;
  bool correct_input = true;
};

arguments FillArguments(int argc, const char **argv);