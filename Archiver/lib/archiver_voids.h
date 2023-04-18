#pragma once
#include <iostream>
#include <vector>
#include <string>

void Append(const std::string &arch_path, const std::string &file_path, uint64_t encoding_freq);

void Extract(const std::string &arch_path, const std::string &file_name, const std::string &mode);

void Delete(const std::string &arch_path, const std::string &file_name, uint64_t encoding_freq);

std::vector<std::string> List(const std::string& arch_path);

void DecodeOneFile(const std::string &string);

void Concatenate(const std::string &arch_path, const std::string &file_path);