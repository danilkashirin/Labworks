#pragma once
#include <iostream>
#include <memory>
#include <stdexcept>

template<typename T>
class Iterator {
 public:
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using iterator_category [[maybe_unused]] = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;

 private:
  pointer start_;
  size_type head_ = 0;
  size_type tail_ = 0;
  int pos_ = 0;
  size_type capacity_ = 0;
  size_type size_ = 0;
  bool begin_ = false;
  bool end_ = false;

 public:
  Iterator() = default;
  Iterator(pointer start,
           int pos,
           size_type head,
           size_type tail,
           size_type size,
           size_type capacity,
           bool begin,
           bool end) {
    start_ = start;
    head_ = head;
    tail_ = tail;
    pos_ = pos;
    capacity_ = capacity;
    size_ = size;
    begin_ = begin;
    end_ = end;
  }

  Iterator operator+=(difference_type value) {
    begin_ = false;
    pos_ = pos_ + value;
    return *this;
  }

  Iterator operator+(difference_type value) {
    Iterator<T> result = *this;
    result += value;
    return result;
  }

  friend Iterator operator+(difference_type value, const Iterator<T>& current) {
    Iterator<T> tmp = current;
    tmp += value;
    return tmp;
  }

  friend difference_type operator+(const Iterator& lhs, const Iterator& rhs) {
    return static_cast<difference_type>(lhs.pos_ + rhs.pos_);
  }

  Iterator operator-=(difference_type value) {
    begin_ = false;
    pos_ = pos_ - value;
    return *this;
  }

  Iterator operator-(difference_type value) {
    Iterator<T> result = *this;
    result -= value;
    return result;
  }

  friend Iterator operator-(difference_type value, const Iterator<T>& current) {
    Iterator<T> tmp = current;
    tmp -= value;
    return tmp;
  }

  friend difference_type operator-(const Iterator& lhs, const Iterator& rhs) {
    return static_cast<difference_type>(lhs.pos_ - rhs.pos_);
  }

  Iterator& operator++() {
    begin_ = false;
    pos_++;
    return *this;
  }

  Iterator& operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  Iterator& operator--() {
    begin_ = false;
    pos_--;
    return *this;
  }

  Iterator& operator--(int) {
    Iterator tmp = *this;
    --(*this);
    return tmp;
  }

  friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
    if (lhs.capacity_ == 0 and rhs.capacity_ == 0) {
      return true;
    }
    if ((lhs.capacity_ == 0 and rhs.capacity_ != 0) or (lhs.capacity_ != 0 and rhs.capacity_ == 0)) {
      return false;
    }
    return ((lhs.pos_ + lhs.head_ + (std::abs(lhs.pos_) / lhs.capacity_ + 1) * lhs.capacity_) % lhs.capacity_ ==
        (rhs.pos_ + rhs.head_ + (std::abs(rhs.pos_) / rhs.capacity_ + 1) * rhs.capacity_) % rhs.capacity_ and
        !(lhs.begin_ and rhs.end_) or (lhs.size_ == 0 and rhs.size_ == 0));
  }

  friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
    return !(lhs == rhs);
  }

  friend bool operator>(const Iterator& lhs, const Iterator& rhs) {
    size_type lhs_position, rhs_position;
    lhs_position = (lhs.pos_ + lhs.head_ + (std::abs(lhs.pos_) / lhs.capacity_ + 1) * lhs.capacity_) % lhs.capacity_;
    rhs_position = (rhs.pos_ + rhs.head_ + (std::abs(rhs.pos_) / rhs.capacity_ + 1) * rhs.capacity_) % rhs.capacity_;
    return lhs_position > rhs_position;
  }

  friend bool operator>=(const Iterator& lhs, const Iterator& rhs) {
    return !(lhs < rhs);
  }

  friend bool operator<(const Iterator& lhs, const Iterator& rhs) {
    size_type lhs_position, rhs_position;
    lhs_position = (lhs.pos_ + lhs.head_ + (std::abs(lhs.pos_) / lhs.capacity_ + 1) * lhs.capacity_) % lhs.capacity_;
    rhs_position = (rhs.pos_ + rhs.head_ + (std::abs(rhs.pos_) / rhs.capacity_ + 1) * rhs.capacity_) % rhs.capacity_;
    return lhs_position < rhs_position;
  }

  friend bool operator<=(const Iterator& lhs, const Iterator& rhs) {
    return !(lhs > rhs);
  }

  pointer operator->() {
    return *(start_[(pos_ + head_ + (std::abs(pos_) / capacity_ + 1) * capacity_) % capacity_]);
  }

  reference operator*() {
    return start_[(pos_ + head_ + (std::abs(pos_) / capacity_ + 1) * capacity_) % capacity_];
  }

  reference operator[](difference_type value) {
    return (start_[(pos_ + head_ + value + (std::abs(pos_ + value) / capacity_ + 1) * capacity_) % capacity_]);
  }
};
