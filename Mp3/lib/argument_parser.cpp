#include <iostream>
#include "argument_parser.h"


arguments FillArguments(int argc, const char** argv) {
  arguments result;
  for(uint64_t i = 1; i < argc; i++) {
    std::string cur_option = argv[i];
    std::string filename;

    if (cur_option == "--list" or cur_option == "-l") {
      result.list = true;
    }

    std::string cur_value;
    int equal_cnt = 0;
    for (auto j : cur_option) {
      if (j == '=') {
        equal_cnt += 1;
      }
      if ((equal_cnt == 1 and j != '=') or (equal_cnt > 1)){
        cur_value.push_back(j);
      }
    }

    if (cur_option[1] == 'p' or cur_option[2] == 'p') {
      result.filename = cur_value;
    }
    if (cur_option[1] == 'f' or cur_option[2] == 'f') {
      result.frame = cur_value;
    }
    if (cur_option[1] == 'r' or cur_option[2] == 'r') {
      result.rewrite.first = cur_value;
    }
    if (cur_option[1] == 'v' or cur_option[2] == 'v') {
      result.rewrite.second = cur_value;
    }
    if (cur_option[1] == 'h' or cur_option[2] == 'h') {
      result.help_frame = cur_value;
    }
  }
  return result;
}
