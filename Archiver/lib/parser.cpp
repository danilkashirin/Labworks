#include "parser.h"
#include <string>

arguments FillArguments(int argc, const char **argv) {
  arguments result;
  uint64_t i = 1;
  while (i < argc) {
    if (argv[i][0] == '-') {
      if ((std::string) argv[i] == "-F" or (std::string) argv[i] == "--freq") {
        i++;
        result.freq = stoi((std::string)argv[i]);
        i++;
      } else if ((std::string) argv[i] == "-c" or (std::string) argv[i] == "--create") {
        i++;
        if ((std::string) argv[i] == "-f") {
          i++;
          result.create.archive_name = (std::string) argv[i];
        } else {
          std::string arch_name = (std::string) argv[i];
          arch_name.erase(0, 7);
          result.create.archive_name = arch_name;
        }
        i++;
        while (i < argc and argv[i][0] != '-') {
          result.create.file_name.push_back((std::string) argv[i]);
          i++;
        }
      } else if ((std::string) argv[i] == "-l" or (std::string) argv[i] == "--list") {
        i++;
        if ((std::string) argv[i] == "-f") {
          i++;
          result.list.archive_name = (std::string) argv[i];
        } else {
          std::string arch_name = (std::string) argv[i];
          arch_name.erase(0, 7);
          result.list.archive_name = arch_name;
        }
        i++;
      } else if ((std::string) argv[i] == "-x" or (std::string) argv[i] == "--extract") {
        i++;
        if ((std::string) argv[i] == "-f") {
          i++;
          result.extract.archive_name = (std::string) argv[i];
        } else {
          std::string arch_name = (std::string) argv[i];
          arch_name.erase(0, 7);
          result.extract.archive_name = arch_name;
        }
        i++;
        while (i < argc and argv[i][0] != '-') {
          result.extract.file_name.push_back((std::string) argv[i]);
          i++;
        }
      } else if ((std::string) argv[i] == "-a" or (std::string) argv[i] == "--append") {
        i++;
        if ((std::string) argv[i] == "-f") {
          i++;
          result.append.archive_name = (std::string) argv[i];
        } else {
          std::string arch_name = (std::string) argv[i];
          arch_name.erase(0, 7);
          result.append.archive_name = arch_name;
        }
        i++;
        while (i < argc and argv[i][0] != '-') {
          result.append.file_name.push_back((std::string) argv[i]);
          i++;
        }
      } else if ((std::string) argv[i] == "-d" or (std::string) argv[i] == "--delete") {
        i++;
        if ((std::string) argv[i] == "-f") {
          i++;
          result.deletee.archive_name = (std::string) argv[i];
        } else {
          std::string arch_name = (std::string) argv[i];
          arch_name.erase(0, 7);
          result.deletee.archive_name = arch_name;
        }
        i++;
        while (i < argc and argv[i][0] != '-') {
          result.deletee.file_name.push_back((std::string) argv[i]);
          i++;
        }
      } else if ((std::string) argv[i] == "-A" or (std::string) argv[i] == "--concatenate") {
        i++;
        while (i < argc and argv[i][0] != '-') {
          result.concatenate.file_name.push_back((std::string) argv[i]);
          i++;
        }
        if ((std::string) argv[i] == "-f") {
          i++;
          result.concatenate.archive_name = argv[i];
        } else if ((std::string) argv[i] == "--file") {
          std::string arch_name = (std::string) argv[i];
          arch_name.erase(0, 7);
          result.deletee.archive_name = arch_name;
        }
        i++;
      }
    }
  }
  return result;
}