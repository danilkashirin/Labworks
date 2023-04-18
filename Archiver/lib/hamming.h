#pragma once
#include <iostream>
#include <vector>

uint64_t HammingLength(uint64_t char_amount);

bool PowerOfTwo(uint64_t number);

std::vector<bool> BinaryRepresentation(unsigned char c);

unsigned char BinaryDecode(const std::vector<bool> &binary);

std::vector<unsigned char> HammingCode(const std::vector<unsigned char> &char_line);

std::vector<unsigned char> HammingDecode(std::vector<unsigned char> code, uint64_t hamming_length);