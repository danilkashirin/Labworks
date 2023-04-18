#include "hamming.h"
#include "archiver_voids.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

void Append(const std::string &arch_path,
            const std::string &file_path,
            const uint64_t encoding_freq) {
  std::ifstream fin;
  fin.open(file_path, std::ios_base::binary);
  std::ofstream fout;
  fout.open(arch_path, std::ios::app);
  char c;
  std::vector<unsigned char> char_line{};
  while (fin.get(c)) {
    char_line.push_back(c);
  }

  if (encoding_freq != 0 and encoding_freq > char_line.size()) {
    std::cout << "ERROR: Frequency is greater than file size.\n";
    return;
  }
  if (encoding_freq == 0) {
    std::cout << "ERROR: Enter encoding frequency.\n";
    return;
  }
  std::vector<uint32_t> frequency_vct{};
  for (int i = 0; i < 20; i++) {
    frequency_vct.push_back(0);
  }
  uint64_t frequency;
  frequency = encoding_freq;
  unsigned int ind = 19;
  while (frequency != 0) {
    frequency_vct[ind] = frequency % 10;
    frequency = frequency / 10;
    ind--;
  }
  for (auto i : frequency_vct) {
    fout << i;
  }

  std::vector<uint32_t> file_size_vct{};
  for (int i = 0; i < 20; i++) file_size_vct.push_back(0);
  uint64_t file_size;
  file_size = sizeof(char) * char_line.size();
  ind = 19;
  while (file_size != 0) {
    file_size_vct[ind] = file_size % 10;
    file_size = file_size / 10;
    ind--;
  }
  for (auto i : file_size_vct) {
    fout << i;
  }

  std::vector<uint32_t> filename_length_vct{};
  for (int i = 0; i < 20; i++) {
    filename_length_vct.push_back(0);
  }
  uint64_t filename_length = file_path.size();
  ind = 19;
  while (filename_length != 0) {
    filename_length_vct[ind] = filename_length % 10;
    filename_length = filename_length / 10;
    ind--;
  }
  for (auto i : filename_length_vct) {
    fout << i;
  }

  std::vector<unsigned char> filename_vct{};
  std::vector<unsigned char> encode_filename_vct{};
  for (int i = 0; i < file_path.size(); i++) {
    filename_vct.push_back(file_path[i]);
  }
  encode_filename_vct = HammingCode(filename_vct);
  for (auto i : encode_filename_vct) {
    fout << i;
  }

  std::vector<unsigned char> char_slice{};
  std::vector<std::vector<unsigned char>> code{};
  for (uint64_t i = 0; i <= char_line.size(); i += encoding_freq) {
    char_slice = {};
    for (int j = 0; j < encoding_freq; j++) {
      char_slice.push_back(char_line[0]);
      char_line.erase(char_line.begin());
    }
    code.push_back(HammingCode(char_slice));
  }

  if (!char_line.empty()) {
    code.push_back(HammingCode(char_line));
  }

  for (const auto &vct : code) {
    for (auto j : vct) {
      fout << j;
    }
  }
  fin.close();
  fout.close();
}

void Extract(const std::string &arch_path, const std::string &file_name, const std::string &mode) {
  std::ifstream fin;
  fin.open(arch_path);
  char c;
  while (fin.get(c)) {
    std::string frequency_str;
    std::string file_size_str;
    std::string filename_length_str;
    uint64_t frequency;
    uint64_t file_size;
    uint64_t filename_length;
    for (int i = 0; i < 60; i++) {
      if (i >= 40) {
        filename_length_str.push_back(c);
      } else if (i >= 20) {
        file_size_str.push_back(c);
      } else {
        frequency_str.push_back(c);
      }
      fin.get(c);
    }
    frequency = std::stoi(frequency_str);
    file_size = std::stoi(file_size_str);
    filename_length = std::stoi(filename_length_str);

    uint64_t encoding_filename_length = HammingLength(filename_length) / 8;
    if (HammingLength(filename_length) % 8 != 0) {
      encoding_filename_length++;
    }
    std::vector<unsigned char> filename_vct{};
    for (int i = 0; i < encoding_filename_length; i++) {
      filename_vct.push_back(c);
      fin.get(c);
    }
    std::vector<unsigned char> name;
    name = HammingDecode(filename_vct, HammingLength(filename_length));

    uint64_t hamming_size = HammingLength(frequency) / 8;
    uint64_t hamming_frequency = HammingLength(frequency) / 8;
    if (HammingLength(frequency) % 8 != 0) {
      hamming_size++;
      hamming_frequency++;
    }
    hamming_size *= file_size / frequency;
    hamming_size += (HammingLength(file_size % frequency) / 8);
    if (HammingLength(file_size % frequency) % 8 != 0) {
      hamming_size++;
    }

    std::vector<unsigned char> hamming_codeline{};
    hamming_codeline.push_back(c);
    for (int i = 0; i < hamming_size - 1; i++) {
      fin.get(c);
      hamming_codeline.push_back(c);
    }

    bool flag = false;
    std::string name_str;
    for (int i = 0; i < name.size(); i++) {
      name_str.push_back(name[i]);
    }
    if (mode == "all" or name_str == file_name) {
      flag = true;
    }
    if (flag) {
      std::ofstream fout;
      fout.open(name_str, std::ios_base::binary);
      std::vector<unsigned char> slice;
      while (hamming_codeline.size() >= hamming_frequency) {
        slice = {};
        for (int i = 0; i < hamming_frequency; i++) {
          slice.push_back(hamming_codeline[0]);
          hamming_codeline.erase(hamming_codeline.begin());
        }
        std::vector<unsigned char> result = HammingDecode(slice, HammingLength(frequency));
        for (auto i : result) fout << i;
      }
      if (!hamming_codeline.empty()) {
        std::vector<unsigned char> result = HammingDecode(hamming_codeline, HammingLength(file_size % frequency));
        for (auto i : result) fout << i;
      }
      fout.close();
    }
  }
  fin.close();
}

void Delete(const std::string &arch_path, const std::string &file_name, const uint64_t encoding_freq) {
  std::string arch_name = arch_path + "1";
  for (auto file : List(arch_path)) {
    if (file != file_name) {
      Append(arch_name, file, encoding_freq);
    }
  }
  remove(arch_path.c_str());
  rename(arch_name.c_str(), arch_path.c_str());
}

std::vector<std::string> List(const std::string &arch_path) {
  std::vector<std::string> result;
  std::ifstream fin;
  fin.open(arch_path);
  char c;
  while (fin.get(c)) {
    std::string frequency_str;
    std::string file_size_str;
    std::string filename_length_str;
    uint64_t frequency;
    uint64_t file_size;
    uint64_t filename_length;
    for (int i = 0; i < 60; i++) {
      if (i >= 40) {
        filename_length_str.push_back(c);
      } else if (i >= 20) {
        file_size_str.push_back(c);
      } else {
        frequency_str.push_back(c);
      }
      fin.get(c);
    }
    frequency = std::stoi(frequency_str);
    file_size = std::stoi(file_size_str);
    filename_length = std::stoi(filename_length_str);

    uint64_t encoding_filename_length = HammingLength(filename_length) / 8;
    if (HammingLength(filename_length) % 8 != 0) {
      encoding_filename_length++;
    }
    std::vector<unsigned char> filename_vct{};
    for (int i = 0; i < encoding_filename_length; i++) {
      filename_vct.push_back(c);
      fin.get(c);
    }
    std::vector<unsigned char> name;
    name = HammingDecode(filename_vct, HammingLength(filename_length));
    std::string name_str;
    for (auto i : name) {
      name_str.push_back(i);
    }
    result.push_back(name_str);

    uint64_t hamming_size = HammingLength(frequency) / 8;
    uint64_t hamming_frequency = HammingLength(frequency) / 8;
    if (HammingLength(frequency) % 8 != 0) {
      hamming_size++;
      hamming_frequency++;
    }
    hamming_size *= file_size / frequency;
    hamming_size += (HammingLength(file_size % frequency) / 8);
    if (HammingLength(file_size % frequency) % 8 != 0) {
      hamming_size++;
    }

    std::vector<unsigned char> hamming_char_code;
    for (int i = 0; i < hamming_size - 1; i++) {
      fin.get(c);
    }
  }
  fin.close();
  return result;
}

void DecodeOneFile(const std::string &arch_path) {
  std::ifstream fin;
  fin.open(arch_path);

  std::string frequency_str;
  std::string file_size_str;
  std::string filename_length_str;
  uint64_t frequency;
  uint64_t file_size;
  uint64_t filename_length;
  char c;
  for (int i = 0; i < 60; i++) {
    fin.get(c);
    if (i >= 40) {
      filename_length_str.push_back(c);
    } else if (i >= 20) {
      file_size_str.push_back(c);
    } else {
      frequency_str.push_back(c);
    }
  }
  frequency = std::stoi(frequency_str);
  file_size = std::stoi(file_size_str);
  filename_length = std::stoi(filename_length_str);

  uint64_t encode_filename_length = HammingLength(filename_length) / 8;
  if (HammingLength(filename_length) % 8 != 0) {
    encode_filename_length++;
  }
  std::vector<unsigned char> filename_vct{};
  for (int i = 0; i < encode_filename_length; i++) {
    fin.get(c);
    filename_vct.push_back(c);
  }
  std::vector<unsigned char> name;
  name = HammingDecode(filename_vct, HammingLength(filename_length));
  for (auto i : name) std::cout << i;
  std::cout << '\n';

  uint64_t hamming_size = HammingLength(frequency) / 8;
  uint64_t hamming_frequency = HammingLength(frequency) / 8;
  if (HammingLength(frequency) % 8 != 0) {
    hamming_size += 1;
    hamming_frequency++;
  }

  hamming_size *= file_size / frequency;
  hamming_size += (HammingLength(file_size % frequency) / 8);
  if (HammingLength(file_size % frequency) % 8 != 0) {
    hamming_size++;
  }

  std::vector<unsigned char> hamming_codeline{};
  for (int i = 0; i < hamming_size; i++) {
    fin.get(c);
    hamming_codeline.push_back(c);
  }
  std::vector<unsigned char> slice;
  while (hamming_codeline.size() >= hamming_frequency) {
    slice = {};
    for (int i = 0; i < hamming_frequency; i++) {
      slice.push_back(hamming_codeline[0]);
      hamming_codeline.erase(hamming_codeline.begin());
    }
    std::vector<unsigned char> result = HammingDecode(slice, HammingLength(frequency));
    for (auto i : result) std::cout << i;
  }

  if (!hamming_codeline.empty()) {
    std::vector<unsigned char> result = HammingDecode(hamming_codeline, HammingLength(file_size % frequency));
    for (auto i : result) std::cout << i;
  }
  fin.close();
}

void Concatenate(const std::string &arch_path, const std::string &file_path) {
  std::ifstream fin;
  fin.open(file_path);
  std::ofstream fout;
  fout.open(arch_path, std::ios::app);
  char c;
  while (fin.get(c)) {
    fout << c;
  }
  fin.close();
  fout.close();
}