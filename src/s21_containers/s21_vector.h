#ifndef S21_CONTAINERS_S21_VECTOR_H_
#define S21_CONTAINERS_S21_VECTOR_H_
#include <algorithm>
#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <utility>

namespace s21 {
template <typename T>
class vector {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

 public:
  vector() = default;

  explicit vector(size_type size) {
    size_ = size;
    capacity_ = size;
    buffer_ = nullptr;
    if (size > 0) {
      buffer_ = new value_type[capacity_];
    }
  }

  vector(std::initializer_list<value_type> const &init)
      : size_{init.size()},
        capacity_(init.size()),
        buffer_{new value_type[capacity_]} {
    std::copy(init.begin(), init.end(), buffer_);
  }

  vector(const vector &cpy) {
    size_ = cpy.size_;
    capacity_ = cpy.capacity_;
    buffer_ = nullptr;
    if (size_ > 0) {
      buffer_ = new value_type[capacity_];
    }
    std::copy(cpy.begin(), cpy.end(), buffer_);
  }

  vector(vector &&mcv) noexcept {
    size_ = std::exchange(mcv.size_, 0);
    capacity_ = std::exchange(mcv.capacity_, 0);
    buffer_ = std::exchange(mcv.buffer_, 0);
  }

  ~vector() { delete[] buffer_; }

  vector &operator=(vector &&mcv) noexcept {
    if (this != &mcv) {
      size_ = std::exchange(mcv.size_, 0);
      capacity_ = std::exchange(mcv.capacity_, 0);
      buffer_ = std::exchange(mcv.buffer_, 0);
    }
    return *this;
  }

  vector &operator=(const vector &mcv) {
    if (this != &mcv) {
      delete[] buffer_;
      if (mcv.size_ > 0) {
        buffer_ = new value_type[mcv.capacity_];
        std::copy(mcv.begin(), mcv.end(), buffer_);
      }
      size_ = mcv.size;
      capacity_ = mcv.capacity;
    }
    return *this;
  }

 public:
  reference at(size_type ind) {
    if (ind >= size_) {
      throw std::logic_error("vector::The index is out of range");
    }
    return buffer_[ind];
  }
  const_reference at(size_type ind) const {
    if (ind > size_) {
      throw std::logic_error("vector::The index is out of range");
    }
    return buffer_[ind];
  }

  constexpr reference operator[](size_type ind) { return at(ind); }

  constexpr const_reference operator[](size_type ind) const { return at(ind); }

  constexpr iterator begin() noexcept { return buffer_; }

  constexpr const_iterator begin() const noexcept { return buffer_; }

  constexpr reference front() {
    if (size_ == 0) throw std::logic_error("zero sized container used");
    return *begin();
  }

  constexpr const_reference front() const {
    if (size_ == 0) throw std::logic_error("zero sized container used");
    return *begin();
  }

  constexpr iterator end() noexcept { return buffer_ + size_; }

  constexpr const_iterator end() const noexcept { return buffer_ + size_; }

  constexpr reference back() {
    if (size_ == 0) throw std::logic_error("Methods on a zero");
    return *std::prev(end());
  }

  constexpr const_reference back() const {
    if (size_ == 0) throw std::logic_error("Methods on a zero");
    return *std::prev(end());
  }

  constexpr iterator data() noexcept { return buffer_; }

  constexpr const_iterator data() const noexcept { return buffer_; }

 public:
  [[nodiscard]] bool empty() const noexcept { return begin() == end(); }

  [[nodiscard]] constexpr size_type size() const noexcept {
    return std::distance(begin(), end());
  }

  [[nodiscard]] constexpr size_type max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(size_type) / 2;
  }

  constexpr void reverse(size_type capacity) {
    if (capacity <= capacity_) {
      return;
    }
    if (capacity > max_size()) {
      throw std::logic_error(
          "reverse capacity is more then max_size value_type");
    }
    reallocVector(capacity);
  }

  constexpr size_type capacity() const noexcept { return capacity_; }

  constexpr void shrink_to_fit() {
    if (capacity_ == size_) {
      return;
    }
    reallocVector(size_);
  }

  constexpr iterator insert(const_iterator ind, value_type &&val) {
    size_type position = ind - begin();
    if (position > size_) {
      throw std::out_of_range("Position is out of range of begin to end");
    }
    if (size_ == capacity_) {
      if (size_)
        reallocVector(2 * size_);
      else
        reallocVector(size_);
    }
    std::copy(begin() + position, end(), begin() + position + 1);
    *(buffer_ + position) = std::move(val);
    ++size_;
    return begin() + position;
  }

  constexpr iterator insert(const_iterator ind, const_reference val) {
    size_type position = ind - begin();
    if (position > size_) {
      throw std::out_of_range("Position is out of range of begin to end");
    }
    if (size_ == capacity_) {
      if (size_)
        reallocVector(2 * size_);
      else
        reallocVector(size_);
    }
    std::copy(begin() + position, end(), begin() + position + 1);
    *(buffer_ + position) = std::move(val);
    ++size_;
    return begin() + position;
  }

  constexpr iterator erase(const_iterator ind) {
    size_type position = ind - begin();

    if (position > size_) {
      throw std::out_of_range("Position is out of range of begin to end");
    }

    std::copy(begin(), const_cast<iterator>(ind), buffer_);
    std::copy(const_cast<iterator>(ind) + 1, end(), buffer_ + position);

    --size_;
    return begin() + position;
  }

  constexpr void push_back(const_reference val) {
    if (size_ == capacity_) {
      if (size_)
        reverse(size_ * 2);
      else
        reverse(size_);
    }
    buffer_[size_] = val;
    ++size_;
  }

  constexpr void push_back(value_type &&val) {
    if (size_ == capacity_) {
      if (size_)
        reverse(size_ * 2);
      else
        reverse(size_);
    }
    buffer_[size_] = std::move(val);
    ++size_;
  }

  constexpr void pop_back() {
    if (size_ == 0) {
      throw std::logic_error("Vector is null, size = 0");
    }
    --size_;
  }

  constexpr void swap(vector &other) noexcept {
    std::swap(buffer_, other.buffer_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  template <typename... Args>
  constexpr iterator emplace(const_iterator ind, Args &&...args) {
    iterator iter = nullptr;
    auto id = ind - begin();
    reverse(capacity_ + sizeof...(args));

    for (auto &&item : {std::forward<Args>(args)...}) {
      iter = insert(begin() + id, item);
    }
    return iter;
  }

  template <typename... Args>
  constexpr iterator emplace_back(Args &&...args) {
    for (auto &&item : {std::forward<Args>(args)...}) {
      push_back(item);
    }
    return end() - 1;
  }

  constexpr void clear() noexcept { size_ = 0; }

 private:
  size_type size_ = 0;
  size_type capacity_ = 0;
  iterator buffer_ = nullptr;

  void reallocVector(size_type capacity) {
    auto temp = new value_type[capacity];
    for (size_type i = 0; i < size_; ++i) temp[i] = std::move(buffer_[i]);
    delete[] buffer_;
    buffer_ = temp;
    capacity_ = capacity;
  }
};
}  // namespace s21
#endif  // S21_CONTAINERS_S21_VECTOR_H_