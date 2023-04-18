#pragma once
#include <iostream>
#include <vector>
#include <string>

struct archive {
  std::string archive_name;
  std::vector<std::string> file_name{};
};

struct arguments {
  archive create;
  archive list;
  archive append;
  archive extract;
  archive deletee;
  archive concatenate;
  uint64_t freq = 0;
};


arguments FillArguments(int argc, const char **argv);