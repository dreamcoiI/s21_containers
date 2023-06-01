#ifndef S21_CONTAINERS_S21_STACK_H
#define S21_CONTAINERS_S21_STACK_H

#include <cstddef>

#include "s21_list.h"

namespace s21 {
template <typename T>
class stack {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  stack() = default;
  stack(std::initializer_list<T> const& items);
  stack(const stack& other);
  stack(stack&& other) noexcept;
  stack& operator=(const stack& other);
  stack& operator=(stack&& other) noexcept;
  ~stack();

  const_reference top();
  bool empty() const;
  size_type size() const;
  void push(const_reference value);
  template <class... Args>
  void emplace(Args&&... args);
  void pop();
  void swap(stack& other);

 private:
  list<value_type> list_;
};

template <typename T>
stack<T>::stack(const stack& other) : list_{other.list_} {}

template <typename T>
stack<T>::stack(std::initializer_list<T> const& items) : list_{items} {}

template <typename T>
stack<T>::stack(stack&& other) noexcept : list_{std::move(other.list_)} {}

template <typename T>
stack<T>& stack<T>::operator=(const stack& other) {
  if (this != &other) {
    stack(other).swap(*this);
  }
  return *this;
}

template <typename T>
stack<T>& stack<T>::operator=(stack&& other) noexcept {
  if (this != &other) {
    list_ = std::move(other.list_);
  }
  return *this;
}

template <typename T>
stack<T>::~stack() {}

template <typename T>
typename stack<T>::const_reference stack<T>::top() {
  return list_.back();
}

template <typename T>
bool stack<T>::empty() const {
  return list_.empty();
}

template <typename T>
typename stack<T>::size_type stack<T>::size() const {
  return list_.size();
}

template <typename T>
void stack<T>::push(const_reference value) {
  list_.push_back(value);
}

template <typename T>
template <class... Args>
void stack<T>::emplace(Args&&... args) {
  list_.emplace_front(std::forward<Args>(args)...);
}

template <typename T>
void stack<T>::pop() {
  list_.pop_back();
}

template <typename T>
void stack<T>::swap(stack& other) {
  list_.swap(other.list_);
}
}  // namespace s21
#endif  // S21_CONTAINERS_S21_STACK_H
