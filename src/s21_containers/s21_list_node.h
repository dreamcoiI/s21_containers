#ifndef S21_CONTAINERS_S21_LIST_NODE_H
#define S21_CONTAINERS_S21_LIST_NODE_H

namespace s21 {
template <typename T>
class list;

template <typename T>
class ListIterator;

template <typename T>
struct Node {
  T value_ = T();
  Node* next_ = nullptr;
  Node* prev_ = nullptr;
  explicit Node(T value = T(), Node* prev = nullptr, Node* next = nullptr)
      : value_(value), next_(next), prev_(prev) {}
};
}  // namespace s21
#endif  // S21_CONTAINERS_S21_LIST_NODE_H
