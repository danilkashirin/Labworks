#pragma once

#include "Column.h"
#include "SqlRequest.h"
#include <memory>
#include <map>
#include <algorithm>
#include <filesystem>
#include <fstream>

class Table {
 public:
  ~Table() = default;
 private:
  friend class Result;
  friend class MyCoolDB;

  bool CheckConditions(const std::vector<Condition>& conditions, size_t row_ind) {
    if (conditions.empty()) {
      return true;
    }

    size_t condition_ind = 0;
    while (condition_ind < conditions.size() and conditions[condition_ind].GetLogicalOperator() != Condition::WHERE) {
      ++condition_ind;
    }

    if (condition_ind >= conditions.size()) {
      return true;
    }

    bool result_check = CheckCondition(conditions[condition_ind], row_ind);
    condition_ind++;
    for (; condition_ind < conditions.size(); ++condition_ind) {
      bool cur_check = CheckCondition(conditions[condition_ind], row_ind);
      auto tmp_logical_operator = conditions[condition_ind].GetLogicalOperator();
      if (tmp_logical_operator == Condition::AND) {
        result_check = result_check and cur_check;
      } else if (tmp_logical_operator == Condition::OR) {
        result_check = result_check or cur_check;
      } else if (tmp_logical_operator == Condition::ON) {
        throw std::invalid_argument("Logical condition error");
      }
    }
    return result_check;
  }
  bool CheckCondition(const Condition& condition, size_t row_ind) {
    if (columns_.contains(condition.GetLhs()) and columns_.contains(condition.GetRhs())) {
      return columns_[condition.GetRhs()]->Compare(*columns_[condition.GetLhs()], condition, row_ind);
    }

    if (columns_.contains(condition.GetLhs())) {
      return columns_[condition.GetLhs()]->Compare(condition.GetRhs(), condition, row_ind);
    }

    if (columns_.contains(condition.GetRhs())) {
      return columns_[condition.GetRhs()]->Compare(condition.GetLhs(), condition, row_ind);
    }

    return true;

  }
  void CreateForeignKey(const std::string& column,
                        const std::string& foreign_key,
                        const std::map<std::string, std::unique_ptr<Table>>& other) {
    auto split_foreign_key = split(foreign_key);
    columns_[column].get()->SetForeignKey(split_foreign_key[0],
                                          other.at(split_foreign_key[0])->columns_.at(split_foreign_key[1]).get());

  };

  template<typename T>
  void AddColumn(const std::string& name, const ElementDataType& type) {
    columns_[name] = std::make_unique<Column<T>>(name, type);
  }
  void AddColumnByType(const std::string& name, const ElementDataType& type) {
    if (type == Bool) {
      AddColumn<bool>(name, type);
    } else if (type == Int) {
      AddColumn<int>(name, type);
    } else if (type == Double) {
      AddColumn<double>(name, type);
    } else if (type == Float) {
      AddColumn<float>(name, type);
    } else if (type == Varchar) {
      AddColumn<std::string>(name, type);
    }
  }

 protected:
  Table() = default;
  std::unordered_map<std::string, std::unique_ptr<BaseColumn>> columns_;
  static std::vector<std::string> split(const std::string& str, char delim = ' ') {
    std::vector<std::string> result;
    std::string token;
    for (char c : str) {
      if (c == delim) {
        result.push_back(token);
        token.clear();
      } else {
        token += c;
      }
    }
    result.push_back(token);
    return result;
  }

 public:
  Table(Table& table, const SqlRequest& query) {
    if (query.GetRequestType() != SqlRequest::Select and query.GetRequestType() != SqlRequest::Join) {
      return;
    }
    auto row = query.GetColumnValues();
    for (const auto& elem : table.columns_) {
      if (row.contains(elem.second->GetName()) or row.contains("*")) {
        AddColumnByType(elem.second->GetName(), elem.second->GetDataType());
      }
    }

    auto conditions = query.GetConditions();
    for (size_t ind = 0; ind < table.GetAmount(); ++ind) {
      if (table.CheckConditions(conditions, ind)) {
        for (auto& elem : columns_) {
          elem.second->CopyData(table.columns_[elem.first].get(), ind);
        }
      }
    }
  }
  explicit Table(const SqlRequest& query, const std::map<std::string, std::unique_ptr<Table>>& other) {
    if (query.GetRequestType() != SqlRequest::Create) {
      return;
    }

    for (auto& elem : query.GetColumnValues()) {
      auto split_column_type = split(elem.second);
      if (split_column_type[0] == "BOOL") {
        AddColumnByType(elem.first, Bool);
      } else if (split_column_type[0] == "INT") {
        AddColumnByType(elem.first, Int);
      } else if (split_column_type[0] == "DOUBLE") {
        AddColumnByType(elem.first, Double);
      } else if (split_column_type[0] == "FLOAT") {
        AddColumnByType(elem.first, Float);
      } else if (split_column_type[0] == "VARCHAR") {
        AddColumnByType(elem.first, Varchar);
      }

      if (std::find(split_column_type.begin(), split_column_type.end(), "PRIMARY") != split_column_type.end()) {
        columns_[elem.first]->SetPrimaryKeyFlag(true);
      }
    }

    for (auto& elem : query.GetColumnValues()) {
      if (elem.first[0] == 'F' and elem.first[1] == ' ') {
        CreateForeignKey(elem.first.substr(2), elem.second, other);
      }
    }
  }

  [[nodiscard]] size_t GetAmount() const {
    return columns_.begin()->second->GetSize();

  }
  [[nodiscard]] std::vector<std::string> GetColumnNames() const {
    std::vector<std::string> result;
    result.reserve(columns_.size());
    for (auto& elem : columns_) {
      result.push_back(elem.first);
    }
    return result;
  }

  void InsertRow(const SqlRequest& query) {
    if (query.GetRequestType() != SqlRequest::Insert) {
      return;
    }
    auto row = query.GetColumnValues();
    for (const auto& elem : columns_) {
      if (row.contains(elem.second->GetName())) {
        int i = 0;
        elem.second->AddData(row[elem.second->GetName()]);
      } else {
        elem.second->SetDefaultData();
      }
    }
  };
  void UpdateRow(const SqlRequest& query) {
    if (query.GetRequestType() != SqlRequest::Update) {
      return;
    }
    auto conditions = query.GetConditions();
    for (size_t i = 0; i < GetAmount(); ++i) {
      if (CheckConditions(conditions, i)) {
        for (auto& elem : query.GetColumnValues()) {
          if (!columns_.contains(elem.first)) {
            throw std::invalid_argument("Update row error");
          }
          columns_[elem.first]->UpdateData(elem.second, i);
        }
      }
    }
  };
  void DeleteRow(const SqlRequest& query) {
    if (query.GetRequestType() != SqlRequest::Delete) {
      return;
    }
    auto conditions = query.GetConditions();
    for (auto ind = static_cast<int64_t>(GetAmount() - 1); ind >= 0; --ind) {
      if (CheckConditions(conditions, ind)) {
        for (auto& elem : columns_) {
          elem.second->DeleteData(ind);
        }
      }
    }
  };

};

class Result : public Table {
 private:
  size_t current_ = SIZE_MAX;

 public:
  Result() = default;
  explicit Result(Table& table, const SqlRequest& query) : Table(table, query) {};

  static Result Join(Table& first_table, Table& second_table, SqlRequest& query) {
    auto join_condition = query.GetColumnValues();
    std::string left_table_name = query.GetTableName() + '.';
    std::string right_table_name = join_condition["JOIN TABLE"] + '.';

    Table result;

    for (auto& elem : first_table.columns_) {
      std::string table_column_name = left_table_name + elem.first;
      if (join_condition.contains(table_column_name) or join_condition.contains("*")) {
        result.AddColumnByType(table_column_name, elem.second->GetDataType());
      }
    }

    for (auto& elem : second_table.columns_) {
      std::string table_column_name = right_table_name + elem.first;
      if (join_condition.contains(table_column_name) or join_condition.contains("*")) {
        result.AddColumnByType(table_column_name, elem.second->GetDataType());
      }
    }

    Table* left_table = &first_table;
    Table* right_table = &second_table;
    if (join_condition["JOIN TYPE"] == "RIGHT") {
      std::swap(left_table, right_table);
      std::swap(left_table_name, right_table_name);
    }

    const bool InnerFlag = (join_condition["JOIN TYPE"] == "INNER");

    std::vector<std::pair<BaseColumn*, BaseColumn*>> conditions;
    conditions.reserve(query.GetConditions().size());
    for (auto& condition : query.GetConditions()) {
      if (condition.GetLogicalOperator() == Condition::WHERE) {
        break;
      }

      std::string left_column = split(condition.GetLhs(), '.')[1];
      std::string right_column = split(condition.GetRhs(), '.')[1];

      if (left_table->columns_.contains(left_column) and right_table->columns_.contains(right_column)) {
        conditions.emplace_back(left_table->columns_[left_column].get(),
                                   right_table->columns_[right_column].get());
      } else if (right_table->columns_.contains(left_column) and left_table->columns_.contains(right_column)) {
        conditions.emplace_back(left_table->columns_[right_column].get(),
                                   right_table->columns_[left_column].get());
      } else {
        throw std::invalid_argument ("Error join condition");
      }
    }

    Condition equals_obj{};
    equals_obj.SetComparisonOperation("=");
    for (size_t i = 0; i < left_table->GetAmount(); ++i) {
      bool joined_flag = false;
      for (auto& elem : conditions) {

        for (size_t j = 0; j < right_table->GetAmount(); ++j) {
          bool equals = true;
          for (auto& current : conditions) {
            auto lhs = current.first;
            auto rhs = current.second;
            if (!lhs->Compare(*rhs, equals_obj, i, j)) {
              equals = false;
              break;
            }
          }

          if (equals) {
            joined_flag = true;
            for (auto& left_table_elem : left_table->columns_) {
              if (result.columns_.contains(left_table_name + left_table_elem.first)) {
                result.columns_[left_table_name + left_table_elem.first]->CopyData(left_table_elem.second.get(), i);
              }
            }

            for (auto& right_table_elem : right_table->columns_) {
              if (result.columns_.contains(right_table_name + right_table_elem.first)) {
                result.columns_[right_table_name + right_table_elem.first]->CopyData(right_table_elem.second.get(), j);
              }
            }
          }
        }
      }

      if (!joined_flag and !InnerFlag) {
        for (auto& left_table_elem : left_table->columns_) {
          if (result.columns_.contains(left_table_name + left_table_elem.first)) {
            result.columns_[left_table_name + left_table_elem.first]->CopyData(left_table_elem.second.get(), i);
          }
        }

        for (auto& right_table_elem : right_table->columns_) {
          result.columns_[right_table_name + right_table_elem.first]->AddData("NULL");
        }
      }
    }
    return Result(result, query);
  };
  bool Next() { return ++current_ < GetAmount(); };
  void Reset() { current_ = 0; }

  template<typename T>
  Cell<T> Get(const std::string& column_name) {
    if (current_ == SIZE_MAX) {
      ++current_;
    }
    if (current_ > GetAmount()) {
      return Cell<T>();
    }

    for (const auto& elem : columns_) {
      if (elem.first == column_name) {
        return static_cast<Column<T>*>(elem.second.get())->GetCell(current_);
      }
    }
    return Cell<T>();
  }

  friend std::ostream& operator<<(std::ostream& os, Result& result) {
    auto column_names = result.GetColumnNames();
    std::reverse_copy(column_names.begin(), column_names.end(), std::ostream_iterator<std::string>(os, " | "));
    os << '\n';
    for (size_t i = 0; i < result.GetAmount(); ++i) {
      for (auto j = column_names.cend() - 1; j >= column_names.cbegin(); --j) {
        os << result.columns_[*j]->GetStringData(i) << " | ";
      }
      os << "\n";
    }
    return os;
  }
};
