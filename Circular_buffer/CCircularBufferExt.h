#pragma once

#include "CCircularBuffer.h"

template<typename T, typename Allocator = std::allocator<T>>

class CCircularBufferExt : public CCircularBuffer<T, Allocator> {
  using CCircularBuffer<T, Allocator>::CCircularBuffer;
 public:
  using value_type = T;
  using reference [[maybe_unused]] = T&;
  using const_reference [[maybe_unused]] = const T&;
  using size_type = size_t;
  using iterator [[maybe_unused]] = Iterator<T>;
  using const_iterstor [[maybe_unused]] = ConstIterator<T>;

  void push_front(const T& value) override {
    if (this->size_ == this->capacity_) {
      if (this->capacity_ == 0) {
        this->reserve(1);
      } else {
        this->reserve(this->capacity_ * kResizeNumber);
      }
    }
    this->head_ = (this->head_ + this->capacity_ - 1) % this->capacity_;
    this->allocator_.construct(this->start_ + this->head_, value);
    this->size_++;
  }

  void push_back(const T& value) override {
    if (this->size_ == this->capacity_) {
      if (this->capacity_ == 0) {
        this->resize(1);
      } else {
        this->resize(  this->capacity_ * kResizeNumber);
      }
    }
    this->allocator_.construct(this->start_ + this->tail_, value);
    this->tail_ = (this->tail_ + 1) % this->capacity_;
    this->size_++;
  }

  Iterator<T> insert(Iterator<T> iterator, T value) {
    if (this->empty()) {
      return iterator;
    }
    for (auto i = this->begin(); i != this->end(); i++) {
      if (i == iterator) {
        size_type cnt = 0;
        CCircularBufferExt<T, Allocator> tmp_buffer(this->capacity_);
        for (auto j = this->begin(); j != iterator; j++) {
          tmp_buffer.push_back(*j);
          cnt++;
        }
        tmp_buffer.push_back(value);
        for (auto j = iterator; j != this->end(); j++) {
          tmp_buffer.push_back(*j);
        }
        *this = tmp_buffer;
        return (this->begin() + cnt);
      }
    }
    return iterator;
  }

  Iterator<T> insert(Iterator<T> iterator, size_type n, T value) {
    if (this->empty() or n == 0) {
      return iterator;
    }
    for (auto i = this->begin(); i != this->end(); i++) {
      if (i == iterator) {
        size_type cnt = 0;
        CCircularBufferExt<T, Allocator> tmp_buffer(this->capacity_);
        for (auto j = this->begin(); j != iterator; j++) {
          tmp_buffer.push_back(*j);
          cnt++;
        }
        for (size_type j = 0; j < n; ++j) {
          tmp_buffer.push_back(value);
        }
        for (auto j = iterator; j != this->end(); j++) {
          tmp_buffer.push_back(*j);
        }
        *this = tmp_buffer;
        return (this->begin() + cnt);
      }
    }
    return iterator;
  }

  Iterator<T> insert(Iterator<T> iterator, std::initializer_list<value_type> list) {
    if (this->empty() or list.size() == 0) {
      return iterator;
    }
    for (auto i = this->begin(); i != this->end(); i++) {
      if (i == iterator) {
        CCircularBufferExt<T, Allocator> tmp_buffer(this->capacity_);
        size_type cnt = 0;
        for (auto j = this->begin(); j != iterator; j++) {
          tmp_buffer.push_back(*j);
          cnt++;
        }
        for (auto j = list.begin(); j != list.end(); j++) {
          tmp_buffer.push_back(*j);
        }
        for (auto j = iterator; j != this->end(); j++) {
          tmp_buffer.push_back(*j);
        }
        *this = tmp_buffer;
        return (this->begin() + cnt);
      }
    }
    return iterator;
  }

  reference operator[](Iterator<T> i) {
    if (i >= this->size_) {
      throw std::out_of_range("Index out of range");
    } else {
      return this->start_[(this->head_ + i) % this->capacity_];
    }
  }

  reference operator[](size_type i) const {
    if (i >= this->size_) {
      throw std::out_of_range("Index out of range");
    } else {
      return this->start_[(this->head_ + i) % this->capacity_];
    }
  }
};