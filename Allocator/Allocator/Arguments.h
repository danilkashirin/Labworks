#pragma once

#include <fstream>
#include <vector>

class Arguments {
 private:
  static size_t pools_cnt_;
  static std::pair<size_t, size_t> default_pool_;
  static std::vector<std::pair<size_t, size_t>> pools_;

 public:
  static void Parse(size_t count_pools, const std::pair<size_t, size_t>& default_pool,
                    const std::vector<std::pair<size_t, size_t>>& pools = {}) {
    pools_cnt_ = count_pools;
    default_pool_ = default_pool;
    pools_ = pools;
  }

  static void Parse(const std::string& filepath) {
    std::ifstream fin;
    fin.open(filepath);
    if (!fin.is_open()) throw std::invalid_argument("filename error");

    fin >> pools_cnt_;
    fin >> default_pool_.first >> default_pool_.second;
    std::pair<size_t, size_t> tmp;

    while(fin >> tmp.first >> tmp.second) {
      pools_.push_back(tmp);
    }
    fin.close();
  }

  static size_t PoolsCnt(){
    return pools_cnt_;
  };

  static std::pair<size_t, size_t> DefaultPool() {
    return default_pool_;
  }

  static std::vector<std::pair<size_t, size_t>> Pools() {
    return pools_;
  }
};

size_t Arguments::pools_cnt_ = 8;
std::pair <size_t, size_t> Arguments::default_pool_ = {8, 256};
std::vector<std::pair<size_t, size_t>> Arguments::pools_ = {};
