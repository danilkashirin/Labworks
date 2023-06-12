#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <regex>

struct Condition {
 public:

  friend class Table;

  enum Logical_Operator {
    AND, OR, WHERE, ON
  };

  enum Comparison_Operator {
    Greater, GreaterOrEqual, Less, LessOrEqual, Equal, NotEqual, Null, NotNull
  };

  explicit Condition(const std::string& condition,
                     const std::string& operation,
                     const std::string& lhs,
                     const std::string& rhs) {
    SetLogicalOperation(condition);
    SetComparisonOperation(operation);
    SetLhs(lhs);
    SetRhs(rhs);
  }

  Condition() = default;
  ~Condition() = default;

  template<typename T>
  bool Comparison(const T& lhs, const T& rhs) const {
    switch (comparison_operator_) {
      case Greater:return lhs > rhs;
      case GreaterOrEqual:return lhs >= rhs;
      case Less:return lhs < rhs;
      case LessOrEqual:return lhs <= rhs;
      case Equal:return lhs == rhs;
      case NotEqual:return lhs != rhs;
      case Null: return false;
      case NotNull: return true;
    }
    return false;
  };

  [[nodiscard]] Logical_Operator GetLogicalOperator() const {
    return logical_operator_;
  };
  [[nodiscard]] Comparison_Operator GetComparisonOperator() const {
    return comparison_operator_;
  };
  [[nodiscard]] std::string GetLhs() const {
    return lhs_;
  };
  [[nodiscard]] std::string GetRhs() const {
    return rhs_;
  };

  void SetLogicalOperation(const std::string& condition) {
    if (condition == "AND") {
      logical_operator_ = AND;
    } else if (condition == "OR") {
      logical_operator_ = OR;
    } else if (condition == "WHERE") {
      logical_operator_ = WHERE;
    } else if (condition == "ON") {
      logical_operator_ = ON;
    }
  };
  void SetComparisonOperation(const std::string& operation) {
    if (operation == ">") {
      comparison_operator_ = Greater;
    } else if (operation == ">=") {
      comparison_operator_ = GreaterOrEqual;
    } else if (operation == "<") {
      comparison_operator_ = Less;
    } else if (operation == "<=") {
      comparison_operator_ = LessOrEqual;
    } else if (operation == "=" or operation == "IS") {
      comparison_operator_ = Equal;
    } else if (operation == "!=") {
      comparison_operator_ = NotEqual;
    }
  };
  void SetLhs(const std::string& lhs) {
    lhs_ = lhs;
  };
  void SetRhs(const std::string& rhs) {
    if (rhs == "NULL" or rhs == "NOT NULL") {
      if (comparison_operator_ != Equal and comparison_operator_ != Null and comparison_operator_ != NotNull) {
        throw std::invalid_argument("SetRhs error");
      }
      comparison_operator_ = (rhs == "NULL") ? Null : NotNull;
    }
    rhs_ = rhs;
  };

 private:
  Logical_Operator logical_operator_ = ON;
  Comparison_Operator comparison_operator_ = Equal;
  std::string lhs_{};
  std::string rhs_{};
};

class SqlRequest {
 public:

  enum RequestType {
    Select, Insert, Update, Delete, Create, Drop, Join, None
  };

  explicit SqlRequest(const std::string& request) {
    if (SelectReq(request)) {
      type_ = Select;
    } else if (InsertReq(request)) {
      type_ = Insert;
    } else if (UpdateReq(request)) {
      type_ = Update;
    } else if (DeleteReq(request)) {
      type_ = Delete;
    } else if (CreateReq(request)) {
      type_ = Create;
    } else if (DropReq(request)) {
      type_ = Drop;
    } else if (JoinReq(request)) {
      type_ = Join;
    } else {
      type_ = None;
      throw std::invalid_argument("Request error");
    }
  }

  [[nodiscard]] RequestType GetRequestType() const {
    return type_;
  }
  [[nodiscard]] std::unordered_map<std::string, std::string> GetColumnValues() const {
    return columns_values_;
  };
  [[nodiscard]] std::string GetTableName() const {
    return table_name_;
  }
  [[nodiscard]] std::vector<Condition> GetConditions() const {
    return conditions_;
  }

 private:
  RequestType type_ = None;
  std::string table_name_;
  std::unordered_map<std::string, std::string> columns_values_;
  std::vector<Condition> conditions_;

  bool SelectReq(const std::string& request) {
    std::regex pattern(R"(SELECT\s+(.+)\s+FROM\s+([^\s]+)\s*(?:(WHERE\s+[^;]+))?[\s;]*)");
    std::smatch matches;

    if (std::regex_match(request, matches, pattern) and matches.size() >= 3) {
      std::string columns_str = matches[1].str();
      if (columns_str == "*") {
        columns_values_["*"] = "";
      } else {
        std::vector<std::string> columns;
        ColumnsValuesReq(columns_str, columns);
        for (auto& column : columns) {
          columns_values_[column] = "";
        }
      }
      table_name_ = matches[2].str();
      if (matches.size() >= 4) {
        ConditionReq(matches[3].str());
      }
      return true;
    }
    return false;
  };
  bool InsertReq(const std::string& request) {
    std::regex
        pattern(R"(INSERT\s+INTO\s+([^\s]+)\s*\(([A-Za-z0-9-_,\s]+)\)\s*VALUES\s*\((['"A-Za-z0-9-_,\s\.]+)\)[\s;]*)");
    std::smatch matches;

    if (std::regex_match(request, matches, pattern) and matches.size() >= 4) {
      table_name_ = matches[1].str();
      std::vector<std::string> columns;
      std::vector<std::string> values;
      ColumnsValuesReq(matches[2].str(), columns);
      ColumnsValuesReq(matches[3].str(), values);
      if (columns.size() != values.size()) {
        throw std::invalid_argument("Error: invalid inset request");
      }
      for (size_t i = 0; i < columns.size(); ++i) {
        columns_values_[columns[i]] = values[i];
      }
      return true;
    }
    return false;
  };
  bool UpdateReq(const std::string& request) {
    std::regex pattern(R"(UPDATE\s+([^\s]+)\s+SET\s+([^(?:WHERE)]+)\s*(?:(WHERE\s+[^;]+))?[\s;]*)");
    std::smatch matches;

    if (std::regex_match(request, matches, pattern) and matches.size() >= 3) {
      table_name_ = matches[1].str();

      std::string columns_str = matches[2].str();
      std::regex columns_pattern(R"(('[^']*')|[^\s=,]+)");

      std::sregex_iterator columns_it(columns_str.begin(), columns_str.end(), columns_pattern);
      std::sregex_iterator columns_end;
      std::string column;
      for (; columns_it != columns_end; ++columns_it) {
        std::string element_str = columns_it->str();
        if (column.empty()) {
          column = element_str;
        } else {
          columns_values_[column] = element_str;
          column = "";
        }
      }
      if (matches.size() >= 4) {
        ConditionReq(matches[3].str());
      }
      return true;
    }
    return false;
  };
  bool DeleteReq(const std::string& request) {
    std::regex pattern(R"(DELETE\s+FROM\s+([^\s]+)\s*(?:(WHERE\s+[^;]+))?;*)");
    std::smatch matches;

    if (std::regex_match(request, matches, pattern) and matches.size() >= 2) {
      table_name_ = matches[1].str();

      if (matches.size() >= 3) {
        ConditionReq(matches[2].str());
      }
      return true;
    }
    return false;
  };
  bool CreateReq(const std::string& request) {
    std::regex pattern(R"(CREATE\s+TABLE\s+([^\s]+)\s+\(([\sA-Za-z0-9-_,\(\)]*)\)[\s;]*)");
    std::smatch matches;

    if (std::regex_match(request, matches, pattern) and matches.size() >= 2) {
      table_name_ = matches[1].str();

      std::string table_struct = matches[2].str();
      std::regex table_struct_pattern(R"(([^\s,\(]+)\s+([^\s,]+(?:\s+(?:PRIMARY KEY))*))");

      std::string::const_iterator start_search(table_struct.cbegin());
      while (std::regex_search(start_search, table_struct.cend(), matches, table_struct_pattern)) {
        if (matches.size() >= 3 and matches[2].str() != "KEY" and matches[2].str() != "REFERENCES") {
          columns_values_[matches[1].str()] = matches[2].str();
        }
        start_search = matches.suffix().first;
      }
      if (columns_values_.empty()) {
        return false;
      }

      std::regex foreign_key_pattern(R"(FOREIGN\s+KEY\s+\(([^\s]+)\)\s+REFERENCES\s+([^\s\)]+)\(([^\s,]+)\))");
      start_search = table_struct.cbegin();
      while (std::regex_search(start_search, table_struct.cend(), matches, foreign_key_pattern)) {
        if (matches.size() >= 4) {
          columns_values_["F " + matches[1].str()] = matches[2].str() + " " + matches[3].str();
          if (!columns_values_.contains(matches[1].str())) {
            return false;
          }
        }
        start_search = matches.suffix().first;
      }
      return true;
    }
    return false;
  };
  bool DropReq(const std::string& request) {
    std::regex pattern(R"(DROP\s+TABLE\s+([^\s;]+)[\s;]*)");
    std::smatch matches;

    if (std::regex_match(request, matches, pattern) and matches.size() >= 2) {
      table_name_ = matches[1].str();
      return true;
    }
    return false;
  };
  bool JoinReq(const std::string& request) {
    std::regex pattern
        (R"(SELECT\s+(.+)\s+FROM\s+([^\s]+)\s+(LEFT|RIGHT|INNER)?\s+JOIN\s+([^\s]+)\s+(ON\s+[^(?:WHERE)]+)\s*(?:(WHERE\s+[^;]+))?[\s;]*)");
    std::smatch matches;

    if (std::regex_match(request, matches, pattern) and matches.size() >= 6) {
      std::string columns_str = matches[1].str();
      if (columns_str == "*") {
        columns_values_["*"] = "";
      } else {
        std::vector<std::string> columns;
        ColumnsValuesReq(columns_str, columns);
        for (auto& column : columns) {
          columns_values_[column] = "";
        }
      }
      table_name_ = matches[2].str();
      columns_values_["JOIN TYPE"] = matches[3].str();
      columns_values_["JOIN TABLE"] = matches[4].str();
      ConditionReq(matches[5].str());

      if (matches.size() >= 7) {
        ConditionReq(matches[6].str());
      }
      return true;
    }
    return false;
  };
  void ConditionReq(const std::string& request) {
    if (request.empty()) {
      return;
    }
    std::regex condition_pattern
        (R"((WHERE|AND|OR|ON)\s+([^\s=]+)\s*([(?:IS)<>!=]+)\s*((?:NOT NULL|NULL)|(?:'[^']*')|(?:[^,\s]+)+))");
    std::smatch matches;

    std::string::const_iterator start_search(request.cbegin());
    while (std::regex_search(start_search, request.cend(), matches, condition_pattern)) {
      if (matches.size() >= 5) {
        Condition condition(matches[1].str(), matches[3].str(), matches[2].str(), matches[4].str());
        conditions_.push_back(condition);
      } else {
        throw std::invalid_argument("Condition Request error");
      }

      start_search = matches.suffix().first;
    }

    if (conditions_.empty()) {
      throw std::invalid_argument("Empty condition");
    }
  };
  static void ColumnsValuesReq(const std::string& str, std::vector<std::string>& container) {
    std::regex pattern(R"(('[^']*')|([^,\s]+))");
    std::sregex_iterator pattern_it(str.begin(), str.end(), pattern);
    std::sregex_iterator pattern_end;

    for (; pattern_it != pattern_end; ++pattern_it) {
      container.push_back(pattern_it->str());
    }
  };
};
