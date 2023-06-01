#ifndef S21_CONTAINERS_S21_QUEUE_H
#define S21_CONTAINERS_S21_QUEUE_H

#include <cstddef>

#include "s21_list.h"

namespace s21 {
template <typename T>
class queue {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  queue() = default;
  queue(const queue& other);
  queue(std::initializer_list<value_type> const& items);
  queue(queue&& other) noexcept;
  queue& operator=(const queue& other);
  queue& operator=(queue&& other) noexcept;
  ~queue();

  const_reference front();
  const_reference back();
  bool empty() const;
  size_type size() const;
  void push(const_reference value);
  void pop();
  void swap(queue& other);
  template <class... Args>
  void emplace_back(Args&&... args);

 private:
  list<value_type> list_;
};

template <typename T>
queue<T>::queue(const queue& other) : list_{other.list_} {}

template <typename T>
queue<T>::queue(std::initializer_list<T> const& items) : list_{items} {}

template <typename T>
queue<T>::queue(queue&& other) noexcept : list_{std::move(other.list_)} {}

template <typename T>
queue<T>& queue<T>::operator=(const queue& other) {
  if (this != &other) {
    queue(other).swap(*this);
  }
  return *this;
}

template <typename T>
queue<T>& queue<T>::operator=(queue&& other) noexcept {
  if (this != &other) {
    list_ = std::move(other.list_);
  }
  return *this;
}

template <typename T>
queue<T>::~queue() {}

template <typename T>
const T& queue<T>::front() {
  return list_.front();
}

template <typename T>
const T& queue<T>::back() {
  return list_.back();
}

template <typename T>
bool queue<T>::empty() const {
  return list_.empty();
}

template <typename T>
typename queue<T>::size_type queue<T>::size() const {
  return list_.size();
}

template <typename T>
void queue<T>::push(const_reference value) {
  list_.push_back(value);
}

template <typename T>
void queue<T>::pop() {
  list_.pop_front();
}

template <typename T>
void queue<T>::swap(queue& other) {
  list_.swap(other.list_);
}

template <typename T>
template <class... Args>
void queue<T>::emplace_back(Args&&... args) {
  list_.emplace_back(std::forward<Args>(args)...);
}
}  // namespace s21
#endif  // S21_CONTAINERS_S21_QUEUE_H
