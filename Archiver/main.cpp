#include "parser.h"
#include "archiver_voids.h"
#include "hamming.h"

int main(int argc, const char **argv) {
  arguments value = FillArguments(argc, argv);
  uint64_t frequency = value.freq;
  if (!value.create.archive_name.empty()) {
    const std::string arch_name = value.create.archive_name;
    remove(arch_name.c_str());
    for (const auto &str : value.create.file_name) {
      std::string file_name = str;
      Append(arch_name, file_name, frequency);
    }
    std::cout << "Archive " << arch_name << " created.\n";
  }
  if (!value.append.archive_name.empty()) {
    std::string arch_name = value.append.archive_name;
    remove(arch_name.c_str());
    for (const auto &str : value.append.file_name) {
      const std::string &file_name = str;
      Append(arch_name, file_name, frequency);
      std::cout << "File " << file_name << " appended.\n";
    }
  }
  if (!value.concatenate.archive_name.empty()) {
    std::string arch_name = value.concatenate.archive_name;
    remove(arch_name.c_str());
    for (const auto &str : value.concatenate.file_name) {
      const std::string &file_name = str;
      Concatenate(arch_name, file_name);
    }
    std::cout << "Archives merged to " << arch_name << ".\n";
  }
  if (!value.list.archive_name.empty()) {
    std::string arch_name = value.list.archive_name;
    std::cout << "Files in archive " << arch_name << ":\n";
    std::vector<std::string> list = List(arch_name);
    for (const auto &str : list) std::cout << str << '\n';
  }
  if (!value.extract.archive_name.empty()) {
    std::string arch_name = value.extract.archive_name;
    if (!value.extract.file_name.empty()) {
      std::string mode = "one";
      for (const auto &file_name : value.extract.file_name) {
        Extract(arch_name, file_name, mode);
        std::cout << "File " << file_name << " extracted from archive " << arch_name << ".\n";
      }
    } else {
      std::string mode = "all";
      Extract(arch_name, "", mode);
      std::cout << "All files extracted from archive " << arch_name << ".\n";
    }
  }
  if (!value.deletee.archive_name.empty()) {
    std::string arch_name = value.deletee.archive_name;
    for (const auto &file_name : value.deletee.file_name) {
      Delete(arch_name, file_name, value.freq);
      std::cout << "File " << file_name << " delete from archive.\n";
    }
  }
}
/*
int main() {
  DecodeOneFile("/Users/danilkasirin/projects_C/Programming/labwork-4-danilkashirin/ARCHIVE");
  //std::cout << HammingCode({'f', 'i', 'l', 'e', '3', '3', '3', '2'}).size() << ' ';
  //std::cout << HammingCode({'3', '4', '5', '6', '7', '8'}).size();
}*/
