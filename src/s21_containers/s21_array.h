#ifndef S21_CONTAINERSPLUS_S21_ARRAY_H
#define S21_CONTAINERSPLUS_S21_ARRAY_H

#include <algorithm>
#include <initializer_list>
#include <stdexcept>

namespace s21 {
template <typename T, std::size_t size_>

class array {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = std::size_t;

 public:
  array() noexcept = default;

  array(std::initializer_list<value_type> const &init) {
    if (init.size() != size_) {
      throw std::logic_error("size not array size");
    }
    for (size_type i = 0; i < size_; ++i) arr_[i] = init.begin()[i];
  }

  array(const array &other) noexcept {
    for (size_type i = 0; i < size_; ++i) arr_[i] = other.arr_[i];
  }

  array &operator=(const array &other) noexcept {
    for (size_type i = 0; i < size_; ++i) arr_[i] = other.arr_[i];
    return *this;
  }

  array &operator=(array &&other) {
    if (this != other) {
      for (auto i = 0; i < size_; ++i) {
        arr_[i] = std::move(other.arr_[i]);
      }
    }
    return *this;
  }

  array(array &&other) noexcept {
    if (this != other) {
      for (size_type i = 0; i < size_; ++i) arr_[i] = std::move(other.arr_[i]);
    }
  }

  ~array() noexcept = default;

 public:
  reference at(size_type ind) {
    if (ind >= size_) {
      throw std::out_of_range("index out of range in array");
    }
    return arr_[ind];
  }

  const_reference at(size_type ind) const {
    if (ind >= size_) {
      throw std::out_of_range("index out of range in array");
    }
    return arr_[ind];
  }

  reference operator[](size_type ind) { return at(ind); }

  const_reference operator[](size_type ind) const { return at(ind); }

  constexpr reference front() {
    if (size_ == 0) {
      throw std::logic_error("size array is null");
    }
    return arr_[0];
  }

  constexpr const_reference front() const {
    if (size_ == 0) {
      throw std::logic_error("size array is null");
    }
    return arr_[0];
  }

  constexpr reference back() {
    if (size_ == 0) {
      throw std::logic_error("size array is null");
    }
    return arr_[size_ - 1];
  }

  constexpr const_reference back() const {
    if (size_ == 0) {
      throw std::logic_error("size array is null");
    }
    return arr_[size_ - 1];
  }

  constexpr iterator data() noexcept { return arr_; }

  constexpr const_iterator data() const noexcept { return arr_; }

  // iterators
 public:
  constexpr iterator begin() noexcept { return arr_; }

  constexpr const_iterator begin() const noexcept { return arr_; }

  constexpr iterator end() noexcept { return arr_ + size_; }

  constexpr const_iterator end() const noexcept { return arr_ + size_; }

  // Capacity
 public:
  [[nodiscard]] constexpr size_type size() const noexcept {
    return std::distance(begin(), end());
  }

  [[nodiscard]] constexpr bool empty() const noexcept {
    return end() == begin();
  }

  [[nodiscard]] constexpr size_type max_size() const noexcept {
    return std::distance(begin(), end());
  }

 public:
  constexpr void swap(array &other) noexcept {
    for (auto start1 = begin(), start2 = other.begin(); start1 != end();
         ++start1, ++start2) {
      T temp = std::move(*start1);
      *start1 = std::move(*start2);
      *start2 = std::move(temp);
    }
  }

  void fill(const_reference val) {
    for (auto *start = begin(); start != end(); ++start) {
      *start = val;
    }
  }

 private:
  value_type arr_[size_] = {};
};
}  // namespace s21

#endif  // S21_CONTAINERSPLUS_S21_ARRAY_H