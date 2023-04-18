#pragma once

#include "Iterator/Iterator.h"
#include "Iterator/ConstIterator.h"
#include <iostream>
#include <memory>
#include <stdexcept>

const uint8_t kResizeNumber = 2;

template <typename T, typename Allocator = std::allocator<T>>
class CCircularBuffer {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using pointer [[maybe_unused]] = T*;
  using size_type = size_t;
  using iterator = Iterator<T>;
  using const_iterstor [[maybe_unused]] = ConstIterator<T>;

 protected:
  T* start_ = nullptr;
  size_type capacity_ = 0;
  size_type size_ = 0;
  size_type head_ = 0;
  size_type tail_ = 0;
  Allocator allocator_;

 public:

  explicit CCircularBuffer(size_type capacity = 0, const Allocator& allocator = Allocator()) {
    capacity_ = capacity;
    allocator_ = allocator;
    start_ = allocator_.allocate(capacity_);
  }

  CCircularBuffer(const CCircularBuffer& tmp_buffer) {
    allocator_ = tmp_buffer.allocator_;
    capacity_ = tmp_buffer.capacity_;
    size_ = tmp_buffer.size_;
    head_ = tmp_buffer.head_;
    tail_ = tmp_buffer.tail_;
    start_ = allocator_.allocate(capacity_);
    if (size_ != 0) {
      size_type i = 0;
      do {
        allocator_.construct(start_ + head_, tmp_buffer[i]);
        head_ = (head_ + 1) % capacity_;
        i++;
      } while (head_ != tail_);
      head_ = tmp_buffer.head_;
    }
  }

  ~CCircularBuffer() {
    clear();
    allocator_.deallocate(start_, capacity_);
  }

  friend bool operator==(CCircularBuffer& lhs, CCircularBuffer& rhs) {
    if (lhs.size_ != rhs.size_ or lhs.capacity_ != rhs.capacity_) {
      return false;
    }
    for (iterator ind_1 = lhs.begin(), ind_2 = rhs.begin(); ind_1 != lhs.end() and ind_2 != rhs.end(); ind_1++, ind_2++) {
      if (*ind_1 != *ind_2) {
        return false;
      }
    }
    return true;
  }

  friend bool operator!=(CCircularBuffer& lhs, CCircularBuffer& rhs) {
    return !(lhs == rhs);
  }

  reference operator[](Iterator<T> i) {
    i = i % capacity_;
    if (i >= size_) {
      throw std::out_of_range("Index out of range");
    } else {
      return start_[(head_ + i) % capacity_];
    }
  }

  virtual reference operator[](size_type i) const {
    i = i % capacity_;
    if (i >= size_) {
      throw std::out_of_range("Index out of range");
    } else {
      return start_[(head_ + i) % capacity_];
    }
  }

  CCircularBuffer& operator=(CCircularBuffer tmp) {
    if (*this != tmp) {
      clear();
      allocator_ = tmp.allocator_;
      if (capacity_ != tmp.capacity_) {
        allocator_.deallocate(start_, capacity_);
        start_ = allocator_.allocate(tmp.capacity_);
        capacity_ = tmp.capacity_;
      }
      size_ = tmp.size_;
      head_ = tmp.head_;
      tail_ = tmp.tail_;
      if (size_ != 0) {
        size_type i = 0;
        do {
          allocator_.construct(start_ + head_, tmp[i]);
          head_ = (head_ + 1) % capacity_;
          i++;
        } while (head_ != tail_);
        head_ = tmp.head_;
      }
    }
    return *this;
  }

  [[maybe_unused]] [[nodiscard]] size_type capacity() const {
    return capacity_;
  }

  [[maybe_unused]] [[nodiscard]] size_type size() const {
    return size_;
  }

  Iterator<T> begin() {
    return Iterator<T>(start_, 0, head_, tail_, size_, capacity_, true, false);
  }

  [[maybe_unused]] Iterator<T> cbegin() const {
    return ConstIterator<T>(start_, 0, head_, tail_, size_, capacity_, true, false);
  }

  Iterator<T> end() {
    return Iterator<T>(start_, size_, head_, tail_, size_, capacity_, false, true);
  }

  [[maybe_unused]] Iterator<T> cend() const {
    return ConstIterator<T>(start_, size_, head_, tail_, size_, capacity_, false, true);
  }

  [[maybe_unused]] reference front() {
    if (empty()) {
      throw std::out_of_range("Empty buffer");
    } else {
      return start_[head_];
    }
  }

  [[maybe_unused]] const_reference const_front() {
    if (!empty()) {
      return start_[head_];
    } else {
      throw std::out_of_range("Empty buffer");
    }
  }

  [[maybe_unused]] reference back() {
    if (!empty()) {
      return start_[(tail_ - 1 + capacity_) % capacity_];
    } else {
      throw std::out_of_range("Empty buffer");
    }
  }

  [[maybe_unused]] const_reference const_back() {
    if (!empty()) {
      return start_[(tail_ - 1 + capacity_) % capacity_];
    } else {
      throw std::out_of_range("Empty buffer");
    }
  }

  [[nodiscard]] bool empty() const {
    return size_ == 0;
  }

  void pop_front() {
    if (!empty()) {
      allocator_.destroy(start_ + head_);
      head_ = (head_ + 1) % capacity_;
      size_--;
    } else {
      throw std::out_of_range("Empty buffer");
    }
  }

  void pop_back() {
    if (!empty()) {
      allocator_.destroy(start_ + tail_);
      tail_ = (tail_ - 1 + capacity_) % capacity_;
      size_--;
    } else {
      throw std::out_of_range("Empty buffer");
    }
  }

  void clear() {
    while (size_ > 0) {
      pop_back();
    }
  }

  [[maybe_unused]] Iterator<T>& erase(Iterator<T>& ind) {
    CCircularBuffer<T, Allocator> tmp_buffer(capacity_);
    Iterator<T> iterator = this->end();
    for (auto j = this->begin(); j != this->end(); j++) {
      if (j != ind) {
        tmp_buffer.push_back(*j);
      } else {
        iterator = j;
      }
    }
    *this = tmp_buffer;
    return iterator;
  }

  [[maybe_unused]] Iterator<T>& erase(Iterator<T>& ind_1, Iterator<T>& ind_2) {
    CCircularBuffer<T, Allocator> tmp_buffer(capacity_);
    Iterator<T> iterator = this->end();
    if (ind_1 >= ind_2) {
      return ind_2;
    }
    for (auto j = this->begin(); j != this->end(); j++) {
      if (ind_1 <= j and j < ind_2) {
        iterator = j;
      } else {
        tmp_buffer.push_back(*j);
      }
    }
    *this = tmp_buffer;
    return iterator;
  }

  void reserve(size_type n) {
    CCircularBuffer<T, Allocator> tmp_buffer(n);
    size_type cnt = 0;
    for (iterator i = this->begin(); i != this->end() && cnt < n; i++, cnt++) {
      tmp_buffer.push_back(*i);
    }
    *this = tmp_buffer;
  }


  void resize(size_type n) {
    size_ = n;
    while (capacity_ < size_) {
      if (capacity_ == 0) {
        reserve(1);
      } else {
        reserve(capacity_ * kResizeNumber);
      }
    }
  }

  virtual void push_back(const T& value) {
    if (size_ == capacity_) {
      pop_front();
    }
    allocator_.construct(start_ + tail_, value);
    tail_ = (tail_ + 1) % capacity_;
    size_++;
  }

  virtual void push_front(const T& value) {
    if (size_ == capacity_) {
      if (empty()) {
        throw std::out_of_range("Empty buffer");
      } else {
        allocator_.destroy(start_ + (head_ + capacity_ - 1) % capacity_);
        tail_ = (tail_ + capacity_ - 1) % capacity_;
        size_--;
      }
    }
    head_ = (head_ + capacity_ - 1) % capacity_;
    allocator_.construct(start_ + head_, value);
    size_++;
  }

  void swap(CCircularBuffer<T, Allocator>& buffer) {
    CCircularBuffer<T, Allocator> tmp_buffer = *this;
    *this = buffer;
    buffer = tmp_buffer;
  }

  virtual Iterator<T> insert(Iterator<T> iterator, T value) {
    if (empty()) {
      return iterator;
    }
    auto result = this->begin();
    for (auto i = this->begin(); i != this->end(); ++i) {
      if (i == iterator) {
        size_type cnt = 0;
        CCircularBuffer tmp_buffer(capacity_);
        for (auto j = this->begin(); j != iterator; j++, cnt++) {
          tmp_buffer.push_back(*j);
        }
        size_type d_1 = cnt;
        tmp_buffer.push_back(value);
        cnt++;
        size_type d_2 = 0;
        int delta = cnt - capacity_;
        if (delta > 0) {
          d_2 = delta % capacity_;
        }
        for (auto j = iterator; j != this->end() and cnt < capacity_; j++, cnt++) {
          tmp_buffer.push_back(*j);
        }
        *this = tmp_buffer;
        return (this->begin() + d_1 - d_2);
      }
    }
    return iterator;
  }

  virtual Iterator<T> insert(Iterator<T> iterator, size_type n, T value) {
    if (this->empty() or n == 0) {
      return iterator;
    }
    for (auto i = this->begin(); i != this->end(); ++i) {
      if (i == iterator) {
        size_type cnt = 0;
        CCircularBuffer<T, Allocator> tmp(capacity_);
        for (auto j = this->begin(); j != iterator; j++, cnt++) {
          tmp.push_back(*j);
        }
        size_type d_1 = cnt;
        for (size_type j = 0; j < n; ++j, cnt++) {
          tmp.push_back(value);
        }
        size_type d_2 = 0;
        int delta = cnt - capacity_;
        if (delta > 0) {
          d_2 = delta % capacity_;
        }
        for (auto j = iterator; j != this->end() and cnt < capacity_; j++, cnt++) {
          tmp.push_back(*j);
        }
        *this = tmp;
        return (this->begin() + d_1 - d_2);
      }
    }
    return iterator;
  }

  virtual Iterator<T> insert(Iterator<T> iterator, std::initializer_list<value_type> list) {
    if (this->empty() or list.size() == 0) {
      return iterator;
    }
    for (auto i = this->begin(); i != this->end(); ++i) {
      if (i == iterator) {
        CCircularBuffer<T, Allocator> tmp(capacity_);
        size_type cnt = 0;
        for (auto j = this->begin(); j != iterator; j++, cnt++) {
          tmp.push_back(*j);
        }
        size_type d_1 = cnt;
        for (auto j = list.begin(); j != list.end(); j++, cnt++) {
          tmp.push_back(*j);
        }
        size_type d_2 = 0;
        int delta = cnt - capacity_;
        if (delta > 0) {
          d_2 = delta % capacity_;
        }
        for (auto j = iterator; j != this->end() and cnt < capacity_; j++, cnt++) {
          tmp.push_back(*j);
        }
        *this = tmp;
        return (this->begin() + d_1 - d_2);
      }
    }
    return iterator;
  }

  void assign(size_type n, T value) {
    CCircularBuffer<T, Allocator> tmp_buffer(n);
    for (int i = 0; i < n; i++) {
      tmp_buffer.push_back(value);
    }
    *this = tmp_buffer;
  }

  void assign(Iterator<T> iterator_1, Iterator<T> iterator_2) {
    size_type cnt = 0;
    for (iterator i = iterator_1; i != iterator_2; i++) {
      cnt++;
    }
    CCircularBuffer<T, Allocator> tmp(cnt);
    for (iterator i = iterator_1; i != iterator_2; i++) {
      tmp.push_back(*i);
    }
    *this = tmp;
  }

  void assign(std::initializer_list<value_type> list) {
    size_type cnt = 0;
    for (auto i = list.begin(); i != list.end(); ++i) {
      cnt++;
    }
    CCircularBuffer<T, Allocator> tmp_buffer(cnt);
    for (auto i = list.begin(); i != list.end(); ++i) {
      tmp_buffer.push_back(*i);
    }
    *this = tmp_buffer;
  }
};
