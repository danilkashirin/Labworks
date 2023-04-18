#pragma once

#include <vector>

template<typename T>
class Pool {
 private:
  std::vector<std::pair<size_t, size_t>> free;
  T* ptr_;
  size_t pieces_cnt_;
  size_t pieces_size_;

 public:
  explicit Pool(const std::pair<size_t, size_t>& current_pool) {
    pieces_cnt_ = current_pool.first;
    pieces_size_ = current_pool.second;
    ptr_ = static_cast<T*>(operator new(size() * sizeof(T)));
    free.emplace_back(0, pieces_cnt_);
  }

  T* allocate(size_t n) {
    size_t allocate_cnt = (n + pieces_size_ - 1) / pieces_size_;
    bool flag = false;
    size_t free_iterator = 0;

    for (int j = 0; j < free.size(); j++) {
      auto i = free[j];
      if (i.second >= allocate_cnt) {
        flag = true;
        free_iterator = i.first;
        if (i.second == allocate_cnt) {
          free.erase(free.begin() + j);
        } else {
          free[j].first += allocate_cnt;
          free[j].second -= allocate_cnt;
        }
        break;
      }
    }

    if (flag) {
      return ptr_ + free_iterator * pieces_size_;
    } else {
      return nullptr;
    }
  }

  bool deallocate(T* ptr, size_t n) {
    if (ptr >= ptr_ and ptr <= (ptr_ + size())) {
      size_t index = ptr - ptr_;
      free.emplace_back(index, n);

      for (int j = 0; j < free.size(); j++) {
        auto i = free[j];
        if (i.first == index + n) {
          free.back().second += i.second;
          free.erase(free.begin() + j);
        }

        if (i.first + i.second == index) {
          free.back().first = free[j].first;
          free.back().second += free[j].second;
          free.erase(free.begin() + j);
        }
      }
      return true;
    } else {
      return false;
    }
  }

  [[nodiscard]] size_t size() const {
    return pieces_cnt_ * pieces_size_;
  }

  [[maybe_unused]] [[nodiscard]] size_t cntPieces() const {
    return pieces_cnt_;
  }

  [[nodiscard]] size_t SizePieces() const {
    return pieces_size_;
  }

  ~Pool() {
    operator delete(ptr_, size() * sizeof(T));
  }
};
