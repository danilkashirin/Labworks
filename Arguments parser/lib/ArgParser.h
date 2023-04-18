#pragma once

#include <iostream>
#include <vector>
#include <variant>

namespace ArgumentParser {

struct IntArgumentBase {
  char short_name = '\0';
  std::string long_name;
  std::string description;

  bool multivalue = false;
  uint64_t multivalue_cnt = 0;
  bool positional = false;

  int value;
  std::vector<int> value_vct{};

  int* store_value;
  std::vector<int>* store_value_vct{};

  bool flag_value = false;
  bool flag_value_vct = false;

  ArgumentParser::IntArgumentBase &Default(int tmp);
  ArgumentParser::IntArgumentBase &StoreValue(int& tmp);
  ArgumentParser::IntArgumentBase &StoreValues(std::vector<int>& tmp);
  ArgumentParser::IntArgumentBase &MultiValue(uint64_t min_args_count = 0);
  ArgumentParser::IntArgumentBase &Positional();
};

struct BoolArgumentBase {
  char short_name = '\0';
  std::string long_name;
  std::string description;

  bool multivalue = false;
  uint64_t multivalue_cnt = 0;
  bool positional = false;

  bool value;
  std::vector<bool> value_vct;

  bool *store_value = nullptr;
  std::vector<bool> *store_value_vct = nullptr;

  bool flag_value = false;
  bool flag_value_vct = false;

  ArgumentParser::BoolArgumentBase &Default(bool tmp);
  ArgumentParser::BoolArgumentBase &StoreValue(bool& tmp);

};

struct StringArgumentBase {
  char short_name = '\0';
  std::string long_name;
  std::string description;

  bool multivalue = false;
  uint64_t multivalue_cnt = 0;
  bool positional = false;

  std::string value;
  std::vector<std::string> value_vct;

  std::string *store_value = nullptr;
  std::vector<std::string> *store_value_vct = nullptr;

  bool flag_value = false;
  bool flag_value_vct = false;

  ArgumentParser::StringArgumentBase &Default(std::string tmp);
  ArgumentParser::StringArgumentBase &StoreValue(std::string& tmp);
  ArgumentParser::StringArgumentBase &StoreValues(std::vector<std::string>& tmp);
  ArgumentParser::StringArgumentBase &MultiValue(uint64_t min_args_count = 0);
  ArgumentParser::StringArgumentBase &Positional();
};

class ArgParser {
 private:
  std::string parser_name;
  std::string short_help;
  std::string long_help;
  std::string parser_description;
  bool help_ = false;
 public:
  std::vector<IntArgumentBase> IntArguments = {};
  std::vector<StringArgumentBase> StrArguments = {};
  std::vector<BoolArgumentBase> BoolArguments = {};

  explicit ArgParser(const std::string &name) {
    parser_name = name;
  }

  bool Parse(const std::vector<std::string> &argv);
  bool Parse(int argc, char **argv);


  IntArgumentBase &AddIntArgument(const char *long_name, const char *description = "");
  IntArgumentBase &AddIntArgument(char short_name, const char *long_name, const char *description = "");
  int GetIntValue(const char *long_name, int i = -1);

  BoolArgumentBase &AddFlag(const char *long_name, const char *description = "");
  BoolArgumentBase &AddFlag(char short_name, const char *long_name, const char *description = "");
  bool GetFlag(const char *long_name, int i = -1);

  StringArgumentBase &AddStringArgument(const char *long_name, const char *description = "");
  StringArgumentBase &AddStringArgument(char short_name, const char *long_name, const char *description = "");
  std::string GetStringValue(const char *long_name, int i = -1);

  void AddHelp(char short_name, const char *long_name, const char *description = "");
  void AddHelp(const char *long_name, const char *description = "");
  [[nodiscard]] bool Help() const;
  std::string HelpDescription();
};
} // namespace ArgumentParser