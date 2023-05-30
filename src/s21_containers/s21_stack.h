#include <cstddef>

#include "s21_list.h"

template <typename T>
class Stack {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  Stack() = default;
  Stack(const Stack& other);
  Stack(Stack&& other) noexcept;
  Stack& operator=(const Stack& other);
  Stack& operator=(Stack&& other) noexcept;
  ~Stack();

  const_reference top() const;
  bool empty() const;
  size_type size() const;
  void push(const_reference value);
  template <class... Args>
  void emplace(Args&&... args);
  void pop();
  void swap(Stack& other);

 private:
  List<value_type> list_;
};

template <typename T>
Stack<T>::Stack(const Stack& other) : list_{other.list_} {}

template <typename T>
Stack<T>::Stack(Stack&& other) noexcept : list_{std::move(other.list_)} {}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& other) {
  if (this != &other) {
    Stack(other).swap(*this);
  }
  return *this;
}

template <typename T>
Stack<T>& Stack<T>::operator=(Stack&& other) noexcept {
  if (this != &other) {
    list_ = std::move(other.list_);
  }
  return *this;
}

template <typename T>
Stack<T>::~Stack() {}

template <typename T>
typename Stack<T>::const_reference Stack<T>::top() const {
  return list_.front();
}

template <typename T>
bool Stack<T>::empty() const {
  return list_.empty();
}

template <typename T>
typename Stack<T>::size_type Stack<T>::size() const {
  return list_.size();
}

template <typename T>
void Stack<T>::push(const_reference value) {
  list_.push_front(value);
}

template <typename T>
template <class... Args>
void Stack<T>::emplace(Args&&... args) {
  list_.emplace_front(std::forward<Args>(args)...);
}

template <typename T>
void Stack<T>::pop() {
  list_.pop_front();
}

template <typename T>
void Stack<T>::swap(Stack& other) {
  list_.swap(other.list_);
}