#pragma once

#include "SqlRequest.h"

#include <utility>
#include <iostream>
#include <vector>
#include <variant>
#include <unordered_map>
#include <string>
#include <set>

size_t kVarcharSize = 255;

template<typename T>
class Cell {
 private:
  T value_ = T();
  bool is_null_ = true;

 public:
  Cell() = default;
  explicit Cell(T value) : value_(value), is_null_(false) {};
  ~Cell() = default;

  [[nodiscard]] T GetValue() const {
    return value_;
  };
  [[nodiscard]] bool IsNull() const {
    return is_null_;
  }

  friend std::ostream& operator<<(std::ostream& os, const Cell& element) {
    if (element.IsNull()) {
      os << "NULL";
    } else if (std::is_same<T, std::string>::value) {
      os << '\'' << element.GetValue() << '\'';
    } else {
      os << element.GetValue();
    }
    return os;
  }

  bool operator==(const Cell& other) const {
    return is_null_ == other.is_null_ and value_ == other.value_;
  }
};

enum ElementDataType {
  Bool, Int, Float, Double, Varchar
};

class BaseColumn {
 protected:
  std::string name_{};
  ElementDataType data_type_{};
  bool primary_key_ = false;

  friend class Result;

 public:

  BaseColumn() = default;

  explicit BaseColumn(std::string name, ElementDataType type) :
      name_(std::move(name)),
      data_type_(type) {};

  virtual ~BaseColumn() = default;

  [[nodiscard]] const std::string& GetName() const {
    return name_;
  }
  [[nodiscard]] std::string GetStructure() const {
    std::string result = name_ + " ";
    if (data_type_ == Bool) {
      result += "BOOL";
    } else if (data_type_ == Int) {
      result += "INT";
    } else if (data_type_ == Float) {
      result += "FLOAT";
    } else if (data_type_ == Double) {
      result += "DOUBLE";
    } else if (data_type_ == Varchar) {
      result += "VARCHAR";
    }

    if (primary_key_) {
      result = result + " " + "PRIMARY KEY";
    }
    return result;
  }

  [[nodiscard]] virtual size_t GetSize() const {
    return 0;
  }
  [[nodiscard]] virtual std::string GetStringData(size_t ind) const {
    return {};
  }
  [[nodiscard]]  ElementDataType GetDataType() const {
    return data_type_;
  }

  virtual void AddData(const std::string& data) {};
  virtual void SetDefaultData() {};
  virtual void UpdateData(const std::string& data, size_t ind) {};
  virtual void DeleteData(size_t ind) {};
  virtual void CopyData(BaseColumn* data, size_t ind) {};

  virtual bool Compare(const BaseColumn& other, const Condition& operation,
                       size_t ind_1, size_t ind_2 = SIZE_MAX) {
    return false;
  }

  virtual bool Compare(const std::string& other, const Condition& operation,
                       size_t ind) {
    return false;
  }

  virtual void SetForeignKey(const std::string& table, BaseColumn* value) {};
  [[nodiscard]] virtual std::string GetForeignKey() const {
    return {};
  }
  void SetPrimaryKeyFlag(bool flag) {
    primary_key_ = flag;
  }
};

template<typename T>
class Column : public BaseColumn {
 private:
  std::vector<Cell<T>> values_{};
  std::pair<std::string, Column<T>*> foreign_key_ = {{}, nullptr};
  Cell<T> default_;

 public:
  Column() = default;
  explicit Column(const std::string& name, ElementDataType type) : BaseColumn(name, type) {
    default_ = Cell<T>();
  };

  [[nodiscard]] Cell<T> GetCell(size_t ind) const {
    return values_[ind];
  }
  [[nodiscard]] size_t GetSize() const override {
    return values_.size();
  }

  [[nodiscard]] std::string GetStringData(size_t ind) const override {
    std::stringstream ss;
    ss << values_[ind];
    return ss.str();
  }

  [[nodiscard]] T ValueConvert(const std::string& data) const;

  void AddData(const std::string& data) override {
    if (data == "NULL") {
      values_.emplace_back();
      return;
    }
    T value = ValueConvert(data);
    CheckData(value);
    values_.emplace_back(value);

  };
  void SetDefaultData() override {
    values_.push_back(default_);
  };
  void UpdateData(const std::string& data, size_t ind) override {
    if (data == "NULL") {
      values_[ind] = Cell<T>();
      return;
    }
    T value = ValueConvert(data);
    CheckData(value);
    values_[ind] = Cell<T>(value);
  }
  void DeleteData(size_t ind) override {
    values_.erase(values_.begin() + ind);
  }
  void CopyData(BaseColumn* data, size_t ind) override {
    values_.push_back(static_cast<Column<T>*>(data)->values_[ind]);
  };

  bool Compare(const BaseColumn& other, const Condition& operation,
               size_t ind_1, size_t ind_2) override {
    if (ind_2 == SIZE_MAX) {
      ind_2 = ind_1;
    }
    return operation.Comparison<T>(values_[ind_1].GetValue(),
                                   static_cast<const Column<T>*>(&other)->values_[ind_2].GetValue());
  }

  bool Compare(const std::string& other, const Condition& operation,
               size_t ind) override {
    Cell<T> element = values_[ind];
    if (operation.GetComparisonOperator() == Condition::Comparison_Operator::Null) {
      return element.IsNull();
    }
    if (operation.GetComparisonOperator() == Condition::Comparison_Operator::NotNull) {
      return !element.IsNull();
    }
    if (element.IsNull()) {
      return false;
    }

    return operation.Comparison<T>(element.GetValue(), ValueConvert(other));
  }

  void CheckData(const T& value) {
    bool foreign_flag = CheckForeignKey(Cell<T>(value));
    if (!foreign_flag or !CheckPrimaryKey(value)) {
      if (foreign_flag) {
        throw std::invalid_argument("Foreign key not unavailable");
      } else {
        throw std::invalid_argument("Primary key not unavailable");
      }
    }
  }
  [[nodiscard]] bool CheckForeignKey(const Cell<T>& tmp) const {
    if (foreign_key_.second == nullptr) {
      return true;
    }
    auto elem = std::find_if(foreign_key_.second->values_.begin(), foreign_key_.second->values_.end(),
                             [tmp](const Cell<T>& cur_elem) { return tmp == cur_elem; });
    return elem != foreign_key_.second->values_.end();
  }
  [[nodiscard]] bool CheckPrimaryKey(const T& tmp) const {
    if (!primary_key_) {
      return true;
    }

    auto elem = std::find_if(values_.begin(), values_.end(),
                             [tmp](const Cell<T>& cur_elem) { return tmp == cur_elem.GetValue(); });
    return elem == values_.end();
  }
  void SetForeignKey(const std::string& table, BaseColumn* value) override {
    foreign_key_ = {table, static_cast<Column<T>*>(value)};
  };
  [[nodiscard]] std::string GetForeignKey() const override {
    if (foreign_key_.second == nullptr) return "";

    return foreign_key_.first + "(" + foreign_key_.second->name_ + ")";
  }
};

template<>
bool Column<bool>::ValueConvert(const std::string& data) const {
  if (data == "1" or data == "true" or data == "True") {
    return true;
  } else if (data == "0" or data == "false" or data == "False") {
    return false;
  } else {
    throw std::invalid_argument("Bool ValueConvert error");
  }
}

template<>
int Column<int>::ValueConvert(const std::string& data) const {
  return std::strtol(data.c_str(), nullptr, 10);
};

template<>
float Column<float>::ValueConvert(const std::string& data) const {
  return std::strtof(data.c_str(), nullptr);
};

template<>
double Column<double>::ValueConvert(const std::string& data) const {
  return std::strtod(data.c_str(), nullptr);
};

template<>
std::string Column<std::string>::ValueConvert(const std::string& data) const {
  return data.substr(1, std::min(data.size() - 2, kVarcharSize));
};
