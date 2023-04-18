#include "parser.h"
#include <string>
#include <iostream>
#include <filesystem>

arguments FillArguments(int argc, const char **argv) {
  arguments result;

  for (int i = 1; i < argc; i++) {
	if (argv[i][0] == '-') {
	  std::string argument = argv[i];
	  if (argument == "-l" or argument == "--length") {
		result.length = std::atoi(argv[i + 1]);
	  } else if (argument == "-w" or argument == "--width") {
		result.width = std::atoi(argv[i + 1]);
	  } else if (argument == "-i" or argument == "--input") {
		result.input = argv[i + 1];
	  } else if (argument == "-o" or argument == "--output") {
		result.output = argv[i + 1];
	  } else if (argument == "-m" or argument == "--max-iter") {
		result.max_iter = std::atoi(argv[i + 1]);
	  } else if (argument == "-f" or argument == "--freq") {
		result.freq = std::atoi(argv[i + 1]);
	  }
	}
  }

  bool input_exist = std::filesystem::exists(std::filesystem::path(result.input));
  bool output_exist = std::filesystem::exists(std::filesystem::path(result.output));
  if (result.length == 0 or result.width == 0 or !input_exist or !output_exist) {
	std::cout << "Invalid input parameters.\n";
	result.correct_input = false;
  }

  return result;
}
