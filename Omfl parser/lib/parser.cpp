#include "parser.h"
#include <fstream>
#include <regex>
#include <vector>

bool ElementTypeValid(omfl::Element& element) {
  using namespace omfl;
  if (element.element_type == Bool and !std::holds_alternative<bool>(element.value) or
      element.element_type == Int and !std::holds_alternative<int>(element.value) or
      element.element_type == Float and !std::holds_alternative<float>(element.value) or
      element.element_type == String and !std::holds_alternative<std::string>(element.value) or
      element.element_type == Vector and !std::holds_alternative<std::vector<Element>>(element.value) or
      element.element_type == None) {
    return false;
  }
  return true;
}

bool CharCriterion(char c) {
  return (std::isalpha(c) or std::isdigit(c) or c == '-' or c == '_');
}

omfl::OmflData omfl::parse(const std::filesystem::path& path) {
  omfl::OmflData current_omfl_data;
  std::ifstream finstream;
  finstream.open(path);

  char cur_c;
  omfl::element_types value_type;
  std::string word;
  std::string tmp_key, tmp_value;
  std::vector<std::string> section_path;
  omfl::Element* cur_section;
  cur_section = current_omfl_data.root_section;

  while (finstream >> word and current_omfl_data.valid_flag) {
    if (word[0] == '[') {
      current_omfl_data.valid_flag = SectionValid(word);
      if (!current_omfl_data.valid_flag) {
        return current_omfl_data;
      }

      section_path = SplitSection(word);
      cur_section = current_omfl_data.root_section;
      for (std::string& str : section_path) {
        auto& map = std::get<std::map<std::string, Element>>(cur_section->value);
        auto iterator = map.find(str);
        if (iterator != map.end()) {
          cur_section = &iterator->second;
        } else {
          cur_section = OmflData::AppendSection(str, cur_section);
        }
      }
      word = "";
    } else if (word[0] == '#') {
      std::getline(finstream, word);
      word = "";
    } else {
      bool in_string_flag = false;
      int quotes_cnt = 0;
      tmp_key = word;
      if (tmp_key[tmp_key.size() - 1] == '=') {
        tmp_key.erase(tmp_key.size() - 1);
      }

      while (true) {
        cur_c = (char) finstream.get();
        if (cur_c == '\n' or cur_c == EOF) {
          break;
        }
        if (cur_c == '#') {
          std::getline(finstream, word);
          word = "";
          break;
        }
        if (cur_c == '\"') {
          in_string_flag = !in_string_flag;
        }
        if (cur_c == '\"') {
          quotes_cnt++;
        }
        if (in_string_flag) {
          tmp_value.push_back(cur_c);
        } else {
          if (cur_c != ' ' and cur_c != '=') {
            tmp_value.push_back(cur_c);
          }
        }
      }

      value_type = ValueType(tmp_value);
      bool criterion = KeyValid(tmp_key) and ValueValid(tmp_value, value_type);
      if (!criterion or quotes_cnt % 2 == 1) {
        current_omfl_data.valid_flag = false;
        return current_omfl_data;
      }


      current_omfl_data.AppendValue(tmp_key, tmp_value, cur_section, value_type);
      if (!current_omfl_data.valid_flag) {
        return current_omfl_data;
      }
      tmp_value = "";
      tmp_key = "";
      word = "";
    }
  }
  finstream.close();
  return current_omfl_data;
}

omfl::OmflData omfl::parse(const std::string& str) {
  std::string file_path = "/Users/danilkasirin/projects_C/Programming/labwork-6-danilkashirin/example/test.omfl";
  std::ofstream file(file_path);
  file.write(str.c_str(), (long)str.size());
  file.close();
  return parse(std::filesystem::path(file_path));
}

bool KeyValid(std::string& key) {
  if (key.empty()) {
    return false;
  }

  if (std::all_of(key.begin(), key.end(), [](char c) { return CharCriterion(c); })) {
    return true;
  } else {
    return false;
  }
}

bool SectionValid(std::string& section) {
  if (section.size() <= 2) {
    return false;
  }

  if (section[0] != '[' or section[section.size() - 1] != ']') {
    return false;
  }

  if (section[1] == '.' or section[section.size() - 2] == '.') {
    return false;
  }

  char prev_c = section[0];
  for (int i = 1; i < section.size(); i++) {
    char cur_c = section[i];
    if (prev_c == '.' and cur_c == '.') {
      return false;
    }
    prev_c = cur_c;
  }

  std::string section_name = section.substr(1, section.size() - 2);
  for (int i = 1; i < section.size() - 1; i++) {
    char cur_c = section[i];
    if (std::isalpha(cur_c) or std::isdigit(cur_c) or cur_c == '-' or cur_c == '_' or cur_c == '.') {
      continue;
    } else {
      return false;
    }
  }

  return true;
}

bool ValueValid(std::string& value, omfl::element_types value_type) {
  using namespace omfl;

  if (value.empty() and value_type != Vector or value_type == None) {
    return false;
  }

  if (value_type != String and value_type != Vector and std::count(value.begin(), value.end(), ' ') != 0) {
    return false;
  }

  if (value_type == Bool) {
    if (value != "true" and value != "false") {
      return false;
    }
  }

  if (value_type == Int) {
    if ((value[0] == '+' or value[0] == '-') and value.size() <= 1) {
      return false;
    }
    if ((value[0] != '+' and value[0] != '-') and !std::isdigit(value[0])) {
      return false;
    }
    for (int i = 1; i < value.size(); i++) {
      if (!std::isdigit(value[i])) {
        return false;
      }
    }
  }

  if (value_type == Float) {
    if (value[0] == '.' or value[value.size() - 1] == '.') {
      return false;
    }
    if ((value[0] == '+' or value[0] == '-') and value.size() <= 1) {
      return false;
    }
    if ((value[0] == '+' or value[0] == '-') and value[1] == '.') {
      return false;
    }

    int point_cnt = 0;
    for (int i = 1; i < value.size(); i++) {
      if (value[i] == '.') point_cnt++;
      if (std::isdigit(value[i]) or value[i] == '.' or point_cnt <= 1) {
        continue;
      } else {
        return false;
      }
    }
  }

  if (value_type == String)
    if (value[0] != '\"' or value[value.size() - 1] != '\"') {
      return false;
    }

  if (value_type == Vector) {
    bool in_str_flag = false;
    bool char_criterion;
    int open_bracket_cnt = 0;
    int close_bracket_cnt = 0;
    for (char cur_c : value) {
      if (cur_c == '\"') {
        in_str_flag = !in_str_flag;
      } else if (cur_c == '[') {
        open_bracket_cnt++;
      } else if (cur_c == ']') {
        close_bracket_cnt++;
      }
      else if (!in_str_flag) {
        char_criterion = std::isdigit(cur_c) or std::isalpha(cur_c);
        std::string special_digits = "+-.,";
        char_criterion = char_criterion or (std::count(special_digits.begin(), special_digits.end(), cur_c) == 1);
        if (!char_criterion) return false;
      }

      if (close_bracket_cnt > open_bracket_cnt){
        return false;
      }
    }
    if (open_bracket_cnt != close_bracket_cnt) {
      return false;
    }
  }

  return true;
}

bool omfl::OmflData::valid() const {
  return valid_flag;
}

std::vector<std::string> SplitSection(std::string& section) {
  std::vector<std::string> sub_sections;
  std::string sct = section;
  sct.push_back('.');
  std::string cur_str;
  for (char c : sct) {
    if (c == '.') {
      sub_sections.emplace_back(cur_str);
      cur_str = "";
    }
    else if (c != '[' and c != ']'){
      cur_str.push_back(c);
    }
  }
  return sub_sections;
}

omfl::Element* omfl::OmflData::AppendSection(std::string& key, omfl::Element* current_section) {
  omfl::Element new_element;
  new_element.element_type = Map;
  std::map<std::string, omfl::Element> cur_map;
  new_element.value = cur_map;
  std::get<std::map<std::string, omfl::Element>>(current_section->value).insert({key, new_element});
  return &(std::get<std::map<std::string, omfl::Element>>(current_section->value).find(key)->second);
}

void omfl::OmflData::AppendValue(std::string& key,
                                           std::string& value,
                                           omfl::Element* current_section,
                                           element_types current_type) {

  if (std::get<std::map<std::string, Element>>(current_section->value).count(key) == 1) {
    valid_flag = false;
  }


  omfl::Element new_element;
  new_element.element_type = current_type;
  if (current_type == Bool) {
    if (value == "true") {
      new_element.value = true;
    }
    else if (value == "false") {
      new_element.value = false;
    }
  }

  else if (current_type == Int) {
    new_element.value = std::stoi(value);
  }

  else if (current_type == Float) {
    new_element.value = std::stof(value);
  }

  else if (current_type == String) {
    new_element.value = value;
  }

  else if (current_type == Vector) {
    new_element.value = AppendVector(value);
  }

  if (valid_flag and !ElementTypeValid(new_element)) {
    valid_flag = false;
  }

  std::get<std::map<std::string, Element>>(current_section->value).insert({key, new_element});
}

std::vector<omfl::Element> omfl::OmflData::AppendVector(std::string& value) {
  std::string& value_vct = value;
  std::vector<Element> tmp_vector;
  std::vector<omfl::Element> result;

  if (value_vct[0] == '[' and value_vct[value_vct.size() - 1] == ']') {
    value_vct = value_vct.substr(1, value_vct.size() - 2);
  }

  bool in_str_flag = false;
  bool in_vct_flag = false;

  element_types tmp_type = None;
  std::string tmp_word;
  value_vct.push_back(',');

  int open_bracket_cnt = 0;
  int close_bracket_cnt = 0;

  int element_char_ind = 0;
  while (element_char_ind < value_vct.size()) {
    char cur_c = value_vct[element_char_ind];
    if (tmp_type == None) {
      if (cur_c == 't' or cur_c == 'f') {
        tmp_type = Bool;
      } else if (std::isdigit(cur_c) or cur_c == '+' or cur_c == '-') {
        tmp_type = Int;
      } else if (cur_c == '\"') {
        tmp_type = String;
      } else if (cur_c == '[') {
        tmp_type = Vector;
      }
    }

    if (tmp_type == Bool) {
      if (cur_c != ',') {
        tmp_word.push_back(cur_c);
      } else {
        Element new_element;
        new_element.element_type = Bool;
        if (tmp_word == "true") {
          new_element.value = true;
        } else if (tmp_word == "false") {
          new_element.value = false;
        }

        if (!ValueValid(tmp_word, tmp_type)) {
          valid_flag = false;
        }

        tmp_type = None;
        tmp_word = "";

        result.push_back(new_element);
      }
    }

    if (tmp_type == Int or tmp_type == Float) {
      if (cur_c == '.') {
        tmp_type = Float;
      }
      if (cur_c != ',') {
        tmp_word.push_back(cur_c);
      } else {
        Element new_element;
        new_element.element_type = tmp_type;
        if (new_element.element_type == Float) {
          new_element.value = std::stof(tmp_word);
        } else if (new_element.element_type == Int) {
          new_element.value = std::stoi(tmp_word);
        }

        if (!ValueValid(tmp_word, tmp_type)) {
          valid_flag = false;
        }

        tmp_type = None;
        tmp_word = "";

        result.push_back(new_element);
      }
    }

    if (tmp_type == String) {
      if (cur_c == '\"') {
        in_str_flag = !in_str_flag;
      }

      if (in_str_flag or cur_c == '\"') {
        tmp_word.push_back(cur_c);
      } else {
        Element new_element;
        new_element.element_type = String;
        new_element.value = tmp_word;

        if (!ValueValid(tmp_word, tmp_type)) {
          valid_flag = false;
        }

        tmp_type = None;
        tmp_word = "";
        in_str_flag = false;

        result.push_back(new_element);
      }
    }

    if (tmp_type == Vector) {
      if (cur_c == '\"') {
        in_str_flag = !in_str_flag;
      }
      if (cur_c == '[' and !in_str_flag) {
        open_bracket_cnt++;
        in_vct_flag = true;
      } else if (cur_c == ']' and !in_str_flag) {
        close_bracket_cnt++;
        in_vct_flag = false;
      }

      if (open_bracket_cnt == close_bracket_cnt and open_bracket_cnt != 0) {
        in_vct_flag = false;
      }
      if (in_vct_flag and cur_c != '[') {
        tmp_word.push_back(cur_c);
      }

      if (!in_vct_flag and cur_c == ',') {
        Element new_element;
        new_element.element_type = tmp_type;
        new_element.value = AppendVector(tmp_word);

        tmp_type = None;
        tmp_word = "";
        in_str_flag = false;
        in_vct_flag = true;
        open_bracket_cnt = 0;
        close_bracket_cnt = 0;

        result.push_back(new_element);
      }
    }
    element_char_ind++;
  }
  return result;
}

omfl::element_types ValueType(const std::string& value) {
  omfl::element_types value_type = omfl::None;
  if (value[0] == '\"') {
    value_type = omfl::String;
  }

  if (std::isdigit(value[0]) or value[0] == '-' or value[0] == '+') {
    value_type = omfl::Int;
    if (std::count(value.begin(), value.end(), '.') == 1) {
      value_type = omfl::Float;
    }
  }

  if (value == "true" or value == "false") {
    value_type = omfl::Bool;
  }

  if (value[0] == '[') {
    value_type = omfl::Vector;
  }
  return value_type;
}

bool omfl::Element::IsString() const {
  if (this->element_type == String) {
    return true;
  }
  return false;
}

bool omfl::Element::IsInt() const {
  if (this->element_type == Int) {
    return true;
  }
  return false;
}

bool omfl::Element::IsFloat() const {
  if (this->element_type == Float) {
    return true;
  }
  return false;
}

bool omfl::Element::IsBool() const {
  if (this->element_type == Bool) {
    return true;
  }
  return false;
}

bool omfl::Element::IsArray() const {
  if (this->element_type == Vector) {
    return true;
  }
  return false;
}

std::string omfl::Element::AsString() const {
  std::string str;
  str = std::get<std::string>(this->value);
  return str.substr(1, str.size() - 2);
}

int omfl::Element::AsInt() const {
  return std::get<int>(this->value);
}

float omfl::Element::AsFloat() const {
  return std::get<float>(this->value);
}

bool omfl::Element::AsBool() const {
  return std::get<bool>(this->value);
}

const omfl::Element& omfl::Element::operator[](int ind) const {
  if ((std::get<std::vector<Element>>(this->value)).size() < ind + 1) {
    auto* default_element = new Element;
    default_element->element_type = Default;
    return *default_element;
  }
  return (std::get<std::vector<Element>>(this->value))[ind];
}

std::string omfl::Element::AsStringOrDefault(const std::string& val) const {
  if (this->element_type == String) {
    return AsString();
  }
  return val;
}

int omfl::Element::AsIntOrDefault(int val) const {
  if (this->element_type == Int) {
    return AsInt();
  }
  return val;
}

float omfl::Element::AsFloatOrDefault(float val) const {
  if (this->element_type == Float) {
    return AsFloat();
  }
  return val;
}

bool omfl::Element::AsBoolOrDefault(bool val) const {
  if (this->element_type == Bool) {
    return AsBool();
  }
  return val;
}

const omfl::Element& omfl::Element::Get(std::string key) const {
  std::vector<std::string> sect_path;
  sect_path = SplitSection(key);
  const omfl::Element* tmp_section;
  tmp_section = this;

  for (int i = 0; i < sect_path.size() - 1; i++) {
    auto sct = sect_path[i];
    tmp_section =& (((std::get<std::map<std::string, Element>>(tmp_section->value)).find(sct))->second);
  }
  std::string last_sct = sect_path[sect_path.size() - 1];
  return (((std::get<std::map<std::string, Element>>(tmp_section->value)).find(last_sct))->second);
}

const omfl::Element& omfl::OmflData::Get(std::string key) const {
  std::vector<std::string> sect_path;
  sect_path = SplitSection(key);
  omfl::Element* tmp_section;
  tmp_section = root_section;
  for (int i = 0; i < sect_path.size() - 1; i++) {
    auto sct = sect_path[i];
    tmp_section =& (((std::get<std::map<std::string, Element>>(tmp_section->value)).find(sct))->second);
  }
  std::string last_sct = sect_path[sect_path.size() - 1];
  return (((std::get<std::map<std::string, Element>>(tmp_section->value)).find(last_sct))->second);
}
