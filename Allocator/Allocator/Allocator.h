#pragma once

#include "Arguments.h"
#include "Sort.h"

#include <stdexcept>
#include <algorithm>

size_t NeedSize = 0;
template<typename T>
bool operator>(const Pool<T>* lhs, const Pool<T>* rhs) {
  if (lhs->size() < NeedSize) {
    return true;
  }

  if (rhs->size() < NeedSize) {
    return false;
  }

  size_t remainder_1 = lhs->SizePieces() - (NeedSize % lhs->SizePieces());
  size_t remainder_2 = rhs->SizePieces() - (NeedSize % rhs->SizePieces());

  if (remainder_1 == remainder_2) {
    return lhs->SizePieces() < rhs->SizePieces();
  } else {
    return remainder_1 > remainder_2;
  }
}

template<typename T>
class Allocator {
 public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using void_pointer = void*;
  using const_void_pointer = const void*;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;

 private:
  std::vector<Pool<value_type>*> pools_;

 public:
  explicit Allocator() {
    pools_.resize(Arguments::PoolsCnt());
    for (size_t i = 0; i < Arguments::PoolsCnt(); i++) {
      if (i < Arguments::Pools().size()) {
        pools_[i] = new Pool<value_type>(Arguments::Pools()[i]);
      } else {
        pools_[i] = new Pool<value_type>(Arguments::DefaultPool());
      }
    }
  }

  Allocator(const Allocator& other) = default;

  pointer allocate(size_t n) {
    NeedSize = n;
    sort(pools_, 0, pools_.size() - 1);
    for (Pool<value_type>* i : pools_) {
      pointer ptr = i->allocate(n);
      if (ptr != nullptr) {
        return ptr;
      }
    }
    throw std::bad_alloc{};
  }

  void deallocate(pointer ptr, size_t n) {
    NeedSize = n;
    sort(pools_, 0, pools_.size() - 1);
    for (Pool<value_type>* i : pools_) {
      if (i->deallocate(ptr, NeedSize)) {
        return;
      }
    }
  }

  ~Allocator() {
    for (Pool<value_type>* elem : pools_) {
      delete elem;
    }
  }
};

template<typename T, typename W>
bool operator==(const Allocator<T>& lhs, const Allocator<W>& rhs) {
  return lhs.pools_ == rhs.pools_;
}

template<typename T, typename W>
bool operator!=(const Allocator<T>& lhs, const Allocator<W>& rhs) {
  return !(lhs == rhs);
}
