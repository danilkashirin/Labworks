#pragma once

#include "Table.h"
#include "SqlRequest.h"

#include <filesystem>
#include <fstream>

class MyCoolDB {
 private:
  std::map<std::string, std::unique_ptr<Table>> tables_;
  Result JoinTables(SqlRequest& request) {
    if (request.GetRequestType() != SqlRequest::Join) {
      return {};
    }
    Result first_table = RequestQuery("SELECT * FROM " + request.GetTableName());
    Result second_table = RequestQuery("SELECT * FROM " + request.GetColumnValues()["JOIN TABLE"]);
    return Result::Join(first_table, second_table, request);
  };

  friend class Table;

 public:
  void Load(const std::filesystem::path& path) {
    std::ifstream fin(path);
    char c;
    std::string query;
    while (fin.get(c)) {
      if (c == ';') {
        Request(query);
        query = {};
        continue;
      } else if (c == '\n') {
        continue;
      }
      query.push_back(c);
    }
    fin.close();
  };
  void Save(const std::filesystem::path& path) {
    std::ofstream fout(path);
    for (auto& elem : tables_) {
      fout << "CREATE TABLE " + elem.first + " (\n";
      for (auto& column : elem.second->columns_) {
        fout << "    " + column.second->GetStructure() + ",\n";
      }

      for (auto& column : elem.second->columns_) {
        if (!column.second->GetForeignKey().empty()) {
          fout << "    FOREIGN KEY (" + column.first + ") REFERENCES" + column.second->GetForeignKey() + ",\n";
        }
      }
      fout << ");\n\n";
    }
    for (auto& elem : tables_) {
      std::string cur_str = "INSERT INTO " + elem.first + "(";

      for (auto& column : elem.second->columns_) {
        cur_str += column.first + ", ";
      }

      cur_str += ") VALUES (";

      for (size_t i = 0; i < elem.second->GetAmount(); ++i) {
        fout << cur_str;

        for (auto& column : elem.second->columns_) {
          fout << column.second->GetStringData(i) + ", ";
        }

        fout << ");\n";
      }

      fout << '\n';
    }

    fout.close();
  };

  void Request(const std::string& request) {
    SqlRequest query(request);

    if (query.GetRequestType() == SqlRequest::Create and tables_.contains(query.GetTableName())) {
      throw std::invalid_argument("Request error - table exists");
    }

    if (query.GetRequestType() != SqlRequest::Create and !tables_.contains(query.GetTableName())) {
      throw std::invalid_argument("Request error - table not exists");
    }

    switch (query.GetRequestType()) {
      case SqlRequest::Insert:
        tables_[query.GetTableName()]->InsertRow(query);
        break;

      case SqlRequest::Update:
        tables_[query.GetTableName()]->UpdateRow(query);
        break;

      case SqlRequest::Delete:
        tables_[query.GetTableName()]->DeleteRow(query);
        break;

      case SqlRequest::Create:
        Table(query, tables_);
        std::make_unique<Table>(query, tables_);
        tables_[query.GetTableName()] = std::make_unique<Table>(query, tables_);
        break;

      case SqlRequest::Drop:
        tables_[query.GetTableName()].reset();
        tables_.erase(query.GetTableName());
        break;

      default:return;
    }
  };

  Result RequestQuery(const std::string& request) {
    SqlRequest query(request);

    if (query.GetRequestType() == SqlRequest::Select) {
      return Result(*tables_[query.GetTableName()], query);
    }
    else if (query.GetRequestType() == SqlRequest::Join) {
      return JoinTables(query);
    }
    return {};
  };

  [[nodiscard]] std::vector<std::string> GetTableNames() const {
    std::vector<std::string> result;
    result.reserve(tables_.size());
    for (auto& elem : tables_) {
      result.push_back(elem.first);
    }
    return result;
  }
};