#pragma once

#include <filesystem>
#include <istream>
#include <variant>
#include <vector>
#include <map>

namespace omfl {
struct Element;

typedef std::variant<bool, int, float, std::string, std::vector<Element>, std::map<std::string, Element>> Type;
//bool - b, int - i, float - f, string - s, vector - v, map (section) - m

enum element_types {
  None, Default, Bool, Int, Float, String, Vector, Map
};

struct Element {
  Type value;
  element_types element_type = None;

  [[nodiscard]] bool IsString() const;
  [[nodiscard]] bool IsInt() const;
  [[nodiscard]] bool IsFloat() const;
  [[nodiscard]] bool IsBool() const;
  [[nodiscard]] bool IsArray() const;

  [[nodiscard]] std::string AsString() const;
  [[nodiscard]] int AsInt() const;
  [[nodiscard]] float AsFloat() const;
  [[nodiscard]] bool AsBool() const;

  [[nodiscard]] const Element& operator[](int ind) const;

  [[nodiscard]] std::string AsStringOrDefault(const std::string& val) const;
  [[nodiscard]] int AsIntOrDefault(int val) const;
  [[nodiscard]] float AsFloatOrDefault(float val) const;
  [[maybe_unused]] [[nodiscard]] bool AsBoolOrDefault(bool val) const;

  [[nodiscard]] const Element& Get(std::string key) const;
};

class OmflData {
 public:
  Element* root_section;
  bool valid_flag = true;
  explicit OmflData() {
    std::map<std::string, Element> new_map;
    root_section = new Element;
    root_section->value = new_map;
    root_section->element_type = Map;
  };

  ~OmflData() {
    delete root_section;
  };

  [[nodiscard]] bool valid() const;
  [[nodiscard]] const Element& Get(std::string key) const;

  void AppendValue(std::string& key, std::string& value, omfl::Element* current_section, element_types type);
  std::vector<omfl::Element> AppendVector(std::string& value);
  static omfl::Element* AppendSection(std::string& key, omfl::Element* current_section);


};

OmflData parse(const std::filesystem::path& path);
OmflData parse(const std::string& str);

}// namespace


static omfl::element_types ValueType(const std::string& value);
static std::vector<std::string> SplitSection(std::string& section);
static bool KeyValid(std::string& key);
static bool SectionValid(std::string& section);
static bool ValueValid(std::string& value, omfl::element_types value_type);