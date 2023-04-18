#include <vector>
#include "hamming.h"
#include <cmath>

///HammmingLength
///
/// \param char_amount
/// count hamming code size by char count
/// \return hamming_size

uint64_t HammingLength(uint64_t char_amount) {
  uint64_t line_size = char_amount * 8;
  uint64_t ind = 0;
  uint64_t hamming_size = 0;
  uint64_t power_of_two = 1;
  while (ind < line_size) {
    if (ind == power_of_two - 1) {
      power_of_two *= 2;
    } else {
      ind++;
    }
    hamming_size++;
  }
  return hamming_size;
}

bool PowerOfTwo(uint64_t number) {
  uint64_t cur = 1;
  while (cur < number) {
    cur *= 2;
  }
  return (cur == number);
}

std::vector<bool> BinaryRepresentation(unsigned char c) {
  std::vector<bool> bin_code{};
  int ind = 128;
  while (ind > 0) {
    bin_code.push_back(bool(c & ind));
    ind = ind >> 1;
  }
  return bin_code;
}

unsigned char BinaryDecode(const std::vector<bool> &binary) {
  int result = 0;
  int base = 128;
  for (auto i : binary) {
    result += (int) i * base;
    base = base / 2;
  }
  return (unsigned char) result;
}

std::vector<unsigned char> HammingCode(const std::vector<unsigned char> &char_line) {
  std::vector<bool> hamming_code{};
  std::vector<bool> binary_code{};
  std::vector<bool> one_char_code{};

  for (auto c : char_line) {
    one_char_code = BinaryRepresentation(c);
    binary_code.insert(binary_code.end(), one_char_code.begin(), one_char_code.end());
  }

  uint64_t ind = 0;
  uint64_t hamming_ind = 0;
  uint64_t power_of_two = 1;
  while (ind < binary_code.size()) {
    if (hamming_ind == power_of_two - 1) {
      hamming_code.push_back(0);
      power_of_two *= 2;
    } else {
      hamming_code.push_back(binary_code[ind]);
      ind++;
    }
    hamming_ind++;
  }

  power_of_two = 1;
  while (power_of_two - 1 < hamming_code.size()) {
    bool sum = 0;
    for (uint64_t j = power_of_two - 1; j < hamming_code.size(); j += 2 * power_of_two) {
      for (uint64_t q = 0; q < power_of_two; q++) {
        if (q + j + 1 > hamming_code.size()) {
          break;
        }
        sum = (sum + hamming_code[std::min(q + j, (uint64_t) hamming_code.size())]) % 2;
      }
    }
    hamming_code[power_of_two - 1] = sum;
    power_of_two = power_of_two * 2;
  }

  std::vector<unsigned char> result;
  std::vector<bool> binary;
  while (hamming_code.size() >= 8) {
    binary = {};
    for (int j = 0; j < 8; j++) {
      binary.push_back(hamming_code[0]);
      hamming_code.erase(hamming_code.begin());
    }
    unsigned char c = BinaryDecode(binary);
    result.push_back(c);
  }

  if (!hamming_code.empty()) {
    std::vector<bool> remaining{};
    for (int i = 0; i < 8 - hamming_code.size(); i++) {
      remaining.push_back(0);
    }
    for (auto j : hamming_code) {
      remaining.push_back(j);
    }
    unsigned char c = BinaryDecode(remaining);
    result.push_back(c);
  }
  return result;
}

std::vector<unsigned char> HammingDecode(std::vector<unsigned char> code, uint64_t hamming_length) {
  std::vector<bool> hamming_code{};
  std::vector<bool> one_char_code{};
  for (uint64_t c = 0; c < code.size(); c++) {
    one_char_code = BinaryRepresentation(code[c]);
    if (c == code.size() - 1) {
      for (int j = 0; j < 8 - hamming_length % 8; j++) {
        one_char_code.erase(one_char_code.begin());
      }
    }
    for (auto i : one_char_code) {
      hamming_code.push_back(i);
    }
  }

  //hamming_code[2] = 1 - hamming_code[2];
  std::vector<bool> binary_code{};
  uint64_t error_ind = 0;
  bool error_flag = false;
  uint64_t power_of_two = 1;
  while (power_of_two - 1 < hamming_code.size()) {
    bool sum = false;
    for (uint64_t j = power_of_two - 1; j < hamming_code.size(); j += 2 * power_of_two) {
      for (uint64_t q = 0; q < power_of_two; q++) {
        if (q + j + 1 > hamming_code.size()) {
          break;
        }
        if (!PowerOfTwo(q + j + 1)) {
          sum = (sum + hamming_code[std::min(q + j, (uint64_t) hamming_code.size())]) % 2;
        }
      }
    }
    if (sum != hamming_code[power_of_two - 1]) {
      error_ind += power_of_two;
      error_flag = true;
    }
    power_of_two = power_of_two * 2;
  }

  if (error_flag) {
    std::cout << "ERROR: The archive is corrupted.\n";
    if (error_ind < hamming_code.size()) {
      hamming_code[error_ind - 1] = 1 - hamming_code[error_ind - 1];
    }
  }

  power_of_two = 1;
  for (int j = 0; j < hamming_code.size(); j++) {
    if (j + 1 == power_of_two) {
      power_of_two *= 2;
    } else {
      binary_code.push_back(hamming_code[j]);
    }
  }

  std::vector<unsigned char> result{};
  std::vector<bool> tmp{};
  for (int j = 0; j < binary_code.size(); j += 8) {
    tmp = {};
    for (int k = 0; k < 8; k++) {
      tmp.push_back(binary_code[j + k]);
    }
    result.push_back(BinaryDecode(tmp));
  }
  return result;
}
