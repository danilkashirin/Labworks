#include "ArgParser.h"

#include <utility>

using namespace ArgumentParser;

ArgumentParser::IntArgumentBase &IntArgumentBase::StoreValue(int& tmp) {
  this->store_value = &tmp;
  return *this;
}

ArgumentParser::BoolArgumentBase &BoolArgumentBase::StoreValue(bool& tmp) {
  this->store_value = &tmp;
  return *this;
}

ArgumentParser::StringArgumentBase &StringArgumentBase::StoreValue(std::string& tmp) {
  this->store_value = &tmp;
  return *this;
}

ArgumentParser::IntArgumentBase &IntArgumentBase::StoreValues(std::vector<int>& tmp) {
  this->store_value_vct = &tmp;
  return *this;
}

ArgumentParser::StringArgumentBase &StringArgumentBase::StoreValues(std::vector<std::string>& tmp) {
  this->store_value_vct = &tmp;
  return *this;
}

ArgumentParser::IntArgumentBase &IntArgumentBase::MultiValue(uint64_t min_args_count) {
  this->multivalue = true;
  this->multivalue_cnt = min_args_count;
  return *this;
}

ArgumentParser::StringArgumentBase &StringArgumentBase::MultiValue(uint64_t min_args_count) {
  this->multivalue = true;
  this->multivalue_cnt = min_args_count;
  return *this;
}

ArgumentParser::IntArgumentBase &IntArgumentBase::Positional() {
  this->positional = true;
  return *this;
}

ArgumentParser::StringArgumentBase &StringArgumentBase::Positional() {
  this->positional = true;
  return *this;
}

ArgumentParser::IntArgumentBase &IntArgumentBase::Default(int tmp) {
  this->flag_value = true;
  this->value = tmp;
  return *this;
}

ArgumentParser::BoolArgumentBase &BoolArgumentBase::Default(bool tmp) {
  this->flag_value = true;
  this->value = tmp;
  return *this;
}

ArgumentParser::StringArgumentBase &StringArgumentBase::Default(std::string tmp) {
  this->flag_value = true;
  this->value = std::move(tmp);
  return *this;
}

bool ArgParser::Parse(const std::vector<std::string> &argv) {
  std::string current_arg_name;
  std::string current_value;

  for (int i = 1; i < argv.size(); i++) {
    std::string word = argv[i];
    if (word == "-h" or word == "--help") {
      return true;
    }

    if (word[0] == '-') {
      if (word[1] == '-') {
        std::size_t equals_ind = word.find('=');
        if (equals_ind != std::string::npos) {
          current_arg_name = word.substr(2, equals_ind - 2);
          current_value = word.substr(equals_ind + 1, word.length() - equals_ind);


          for (StringArgumentBase &arg : StrArguments) {
            if (arg.long_name == current_arg_name) {
              if (!arg.multivalue) {
                arg.value = current_value;
                if (arg.store_value != nullptr) {
                  *arg.store_value = arg.value;
                }
                arg.flag_value = true;
              } else if (arg.multivalue) {
                arg.value_vct.emplace_back(current_value);
                if (arg.store_value_vct != nullptr) {
                  *arg.store_value_vct = arg.value_vct;
                }
                arg.flag_value_vct = true;
              }
            }
          }

          for (IntArgumentBase &arg : IntArguments) {
            if (arg.long_name == current_arg_name) {
              if (!arg.multivalue) {
                arg.value = std::stoi(current_value);
                if (arg.store_value != nullptr) {
                  *arg.store_value = arg.value;
                }
                arg.flag_value = true;
              } else if (arg.multivalue) {
                arg.value_vct.emplace_back(std::stoi(current_value));
                if (arg.store_value_vct != nullptr) {
                  if (arg.store_value_vct != nullptr) {
                    *arg.store_value_vct = arg.value_vct;
                  }
                }
                arg.flag_value_vct = true;
              }
            }
          }

          for (BoolArgumentBase &arg : BoolArguments) {
            if (arg.long_name == current_arg_name) {
              if (current_value == "true" or current_value == "1") {
                arg.value = true;
                if (arg.store_value != nullptr) {
                  *arg.store_value = arg.value;
                }
                arg.flag_value = true;
              } else if (current_value == "false" or current_value == "0") {
                arg.value = false;
                if (arg.store_value != nullptr) {
                  *arg.store_value = arg.value;
                }
                arg.flag_value = true;
              } else {
                return false;
              }
            }
          }
        } else {
          current_arg_name = word.substr(2, word.length());
          for (IntArgumentBase &arg : IntArguments) {
            if (arg.long_name == current_arg_name) {
              return false;
            }
          }
          for (StringArgumentBase &arg : StrArguments) {
            if (arg.long_name == current_arg_name) {
              return false;
            }
          }
          for (BoolArgumentBase &arg : BoolArguments)
            if (arg.long_name == current_arg_name) {
              arg.value = true;
              if (arg.store_value != nullptr) {
                *arg.store_value = arg.value;
              }
              arg.flag_value = true;
            }
        }
      } else if (word[1] != '-') {
        std::size_t equals_ind = word.find('=');
        if (equals_ind != std::string::npos) {
          current_arg_name = word.substr(1, equals_ind - 1);
          current_value = word.substr(equals_ind + 1, word.length() - equals_ind);
          if (current_arg_name.length() != 1) {
            return false;
          }

          for (IntArgumentBase &arg : IntArguments) {
            if (arg.short_name == current_arg_name[0]) {
              if (!arg.multivalue) {
                arg.value = std::stoi(current_value);
                if (arg.store_value != nullptr) {
                  *arg.store_value = arg.value;
                }
                arg.flag_value = true;
              } else if (arg.multivalue) {
                arg.value_vct.emplace_back(std::stoi(current_value));
                if (arg.store_value_vct != nullptr) {
                  if (arg.store_value_vct != nullptr) {
                    *arg.store_value_vct = arg.value_vct;
                  }
                }
                arg.flag_value_vct = true;
              }
            }
          }

          for (StringArgumentBase &arg : StrArguments) {
            if (arg.short_name == current_arg_name[0]) {
              if (!arg.multivalue) {
                arg.value = current_value;
                if (arg.store_value != nullptr) {
                  *arg.store_value = arg.value;
                }
                arg.flag_value = true;
              } else if (arg.multivalue) {
                arg.value_vct.emplace_back(current_value);
                if (arg.store_value_vct != nullptr) {
                  if (arg.store_value_vct != nullptr) {
                    *arg.store_value_vct = arg.value_vct;
                  }
                }
                arg.flag_value_vct = true;
              }
            }
          }
          for (BoolArgumentBase &arg : BoolArguments) {
            if (arg.short_name == current_arg_name[0]) {
              if (current_value == "true" or current_value == "1") {
                arg.value = true;
                if (arg.store_value != nullptr) {
                  *arg.store_value = arg.value;
                }
                arg.flag_value = true;
              } else if (current_value == "false" or current_value == "0") {
                arg.value = false;
                if (arg.store_value != nullptr) {
                  *arg.store_value = arg.value;
                }
                arg.flag_value = true;
              } else {
                return false;
              }
            }
          }
        } else {
          current_arg_name = word.substr(1, word.length());
          for (auto flag : current_arg_name) {
            for (auto &arg : BoolArguments) {
              if (arg.short_name == flag) {
                arg.value = true;
                if (arg.store_value != nullptr) {
                  *arg.store_value = arg.value;
                }
                arg.flag_value = true;
                break;
              }
            }
          }
        }
      }

    } else if (word[0] != '-') {
      current_value = word;
      for (auto &arg : IntArguments) {
        if (!arg.multivalue) {
          arg.value = std::stoi(current_value);
          if (arg.store_value != nullptr) {
            *arg.store_value = arg.value;
          }
          arg.flag_value = true;
        } else if (arg.multivalue) {
          arg.value_vct.emplace_back(std::stoi(current_value));
          if (arg.store_value_vct != nullptr) {
            if (arg.store_value_vct != nullptr) {
              *arg.store_value_vct = arg.value_vct;
            }
          }
          arg.flag_value = true;
        }
      }
      for (auto &arg : StrArguments) {
        if (!arg.multivalue) {
          arg.value = current_value;
          if (arg.store_value != nullptr) {
            *arg.store_value = arg.value;
          }
          arg.flag_value = true;
        } else if (arg.multivalue) {
          arg.value_vct.emplace_back(current_value);
          if (arg.store_value_vct != nullptr) {
            if (arg.store_value_vct != nullptr) {
              *arg.store_value_vct = arg.value_vct;
            }
          }
          arg.flag_value = true;
        }
      }
      for (auto &arg : BoolArguments) {
        arg.value = true;
        if (arg.store_value != nullptr) {
          *arg.store_value = arg.value;
        }
        arg.flag_value = true;
      }
    }
  }

  for (auto& arg : StrArguments) {
    if (arg.multivalue and arg.value_vct.size() < arg.multivalue_cnt) {
      return false;
    }
    if (!arg.flag_value and !arg.flag_value_vct) {
      return false;
    }
  }

  for (auto& arg : IntArguments) {
    if (arg.multivalue and arg.value_vct.size() < arg.multivalue_cnt) {
      return false;
    }
    if (!arg.flag_value and !arg.flag_value_vct) {
      return false;
    }
  }
  return true;
}

bool ArgParser::Parse(int argc, char **argv) {
  std::vector<std::string> argv_vct(argc);
  for (int i = 0; i < argc; i++) {
    argv_vct[i] = argv[i];
  }
  return Parse(argv_vct);
}

IntArgumentBase &ArgParser::AddIntArgument(char short_name, const char *long_name, const char *description) {
  IntArgumentBase new_argument;
  new_argument.short_name = short_name;
  new_argument.long_name = long_name;
  new_argument.description = description;
  IntArguments.push_back(new_argument);
  return IntArguments.back();
}

IntArgumentBase &ArgParser::AddIntArgument(const char *long_name, const char *description) {
  return AddIntArgument('\0', long_name, description);
}

BoolArgumentBase &ArgParser::AddFlag(char short_name, const char *long_name, const char *description) {
  BoolArgumentBase new_argument;
  new_argument.short_name = short_name;
  new_argument.long_name = long_name;
  new_argument.description = description;
  BoolArguments.push_back(new_argument);
  return BoolArguments.back();
}

BoolArgumentBase &ArgParser::AddFlag(const char *long_name, const char *description) {
  return AddFlag('\0', long_name, description);
}

StringArgumentBase &ArgParser::AddStringArgument(char short_name, const char *long_name, const char *description) {
  StringArgumentBase new_argument;
  new_argument.short_name = short_name;
  new_argument.long_name = long_name;
  new_argument.description = description;
  StrArguments.push_back(new_argument);
  return StrArguments.back();
}

StringArgumentBase &ArgParser::AddStringArgument(const char *long_name, const char *description) {
  return AddStringArgument('\0', long_name, description);
}

void ArgParser::AddHelp(char short_name, const char *long_name, const char *description) {
  help_ = true;
  short_help = short_name;
  long_help = long_name;
  parser_description = description;
}

void ArgParser::AddHelp(const char *long_name, const char *description) {
  return AddHelp('\0', long_name, description);
}

bool ArgParser::Help() const {
  return help_;
}

int ArgParser::GetIntValue(const char *long_name, int i) {
  int tmp;
  for (IntArgumentBase arg : IntArguments) {
    if (arg.long_name == long_name) {
      if (i == -1) {
        tmp = arg.value;
      } else {
        tmp = arg.value_vct[i];
      }
    }
  }
  return tmp;
}

bool ArgParser::GetFlag(const char *long_name, int i) {
  bool tmp;
  for (BoolArgumentBase arg : BoolArguments) {
    if (arg.long_name == long_name) {
      if (i == -1) {
        tmp = arg.value;
      } else {
        tmp = arg.value_vct[i];
      }
    }
  }
  return tmp;
}

std::string ArgParser::GetStringValue(const char *long_name, int i) {
  std::string tmp;
  for (StringArgumentBase arg : StrArguments) {
    if (arg.long_name == long_name) {
      if (i == -1) {
        tmp = arg.value;
      } else {
        tmp = arg.value_vct[i];
      }
    }
  }
  return tmp;
}

std::string ShortHelpDescription(const IntArgumentBase& arg) {
  std::string argument_description;
  if (arg.short_name != '\0') {
    std::string current_str(1, arg.short_name);
    argument_description = current_str + ", ";
  }
  argument_description += arg.long_name + ", ";
  argument_description += arg.description + "\n";
  return argument_description;
}

std::string ShortHelpDescription(const StringArgumentBase& arg) {
  std::string argument_description;
  if (arg.short_name != '\0') {
    std::string current_str(1, arg.short_name);
    argument_description = current_str + ", ";
  }
  argument_description += arg.long_name + ", ";
  argument_description += arg.description + "\n";
  return argument_description;
}

std::string ShortHelpDescription(const BoolArgumentBase& arg) {
  std::string argument_description;
  if (arg.short_name != '\0') {
    std::string current_str(1, arg.short_name);
    argument_description = current_str + ", ";
  }
  argument_description += arg.long_name + ", ";
  argument_description += arg.description + "\n";
  return argument_description;
}

std::string ArgParser::HelpDescription() {
  std::string help_description;
  help_description = parser_name + '\n' + parser_description + '\n' + '\n';
  std::string argument_description;
  for (const auto& arg : IntArguments) {
    help_description += ShortHelpDescription(arg);
  }

  for (const auto& arg : StrArguments) {
    help_description += ShortHelpDescription(arg);
  }

  for (const auto& arg : BoolArguments) {
    help_description += ShortHelpDescription(arg);
  }
  return help_description;
}