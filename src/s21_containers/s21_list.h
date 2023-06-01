#ifndef S21_CONTAINERS_S21_LIST_H
#define S21_CONTAINERS_S21_LIST_H

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <limits>
#include <utility>

#include "iterators/s21_list_iterator.h"
#include "s21_list_node.h"

namespace s21 {
template <typename T>
class list {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = ListIterator<T>;
  using const_iterator = const iterator;
  using size_type = size_t;

  list() { end_.node_ = new Node(value_type()); }

  list(size_type n) : list() {
    for (size_type i = 0; i < n; i += 1) {
      push_back(value_type());
    }
  }

  list(std::initializer_list<value_type> const &items) : list() {
    for (const auto &i : items) {
      push_back(i);
    }
  }

  list(const list &l) : list() { operator=(l); }

  list(list &&l) : list() {
    if (this != &l) {
      clear();
      size_ = l.size_;
      begin_ = l.begin_;
      end_.node_->prev_ = l.end_.node_->prev_;
      if (end_.node_->prev_ != nullptr) {
        end_.node_->prev_->next_ = end_.node_;
      }
      l.size_ = 0;
      l.end_.node_->prev_ = nullptr;
    }
  }

  ~list() {
    clear();
    delete end_.node_;
  }

  list &operator=(const list &l) {
    if (this != &l) {
      clear();
      Node<T> *buf = l.begin_.node_;
      while (buf != l.end_.node_) {
        push_back(buf->value_);
        buf = buf->next_;
      }
    }
    return *this;
  }

  const_reference front() {
    if (begin_.node_ == nullptr) {
      throw "Collection is empty";
    }
    return (*begin_);
  }

  const_reference back() {
    if (begin_.node_ == nullptr) {
      throw "Collection is empty";
    }
    return (end_.node_->prev_->value_);
  }

  iterator begin() {
    if (empty()) {
      return end_;
    } else {
      return begin_;
    }
  }

  iterator end() { return end_; }

  const_iterator begin() const {
    if (empty()) {
      return const_iterator(end_.node_);
    } else {
      return const_iterator(begin_.node_);
    }
  }

  const_iterator end() const { return const_iterator(end_.node_); }

  bool empty() const noexcept { return size_ == 0; }

  size_type size() const { return size_; }

  size_type max_size() const noexcept {
    return (std::numeric_limits<size_type>::max() / sizeof(Node<T>) / 2);
  }

  void clear() {
    while (!empty()) {
      pop_back();
    }
    end_.node_->prev_ = nullptr;
  }

  iterator insert(iterator pos, const_reference value) {
    if (size_ + 1 > max_size()) throw "Maximum of container";
    iterator res;
    if (pos == end_) {
      if (end_.node_->prev_ == nullptr) {
        begin_.node_ = new Node<value_type>(value, nullptr, end_.node_);
        end_.node_->prev_ = begin_.node_;
        res = begin_;
      } else {
        auto buf = end_.node_->prev_;
        end_.node_->prev_ =
            new Node<value_type>(value, end_.node_->prev_, end_.node_);
        buf->next_ = end_.node_->prev_;
        res = iterator(end_.node_->prev_);
      }
    } else {
      if (pos == begin_) {
        begin_.node_ = new Node<value_type>(value, nullptr, pos.node_);
        pos.node_->prev_ = begin_.node_;
        res = begin_;
      } else {
        pos.node_->prev_->next_ =
            new Node<value_type>(value, pos.node_->prev_, pos.node_);
        pos.node_->prev_ = pos.node_->prev_->next_;
        res = iterator(pos.node_->prev_);
      }
    }
    size_ += 1;
    return res;
  }

  void erase(iterator pos) {
    if (size_ != 0 && pos.node_ != nullptr && pos != end_) {
      if (pos == begin_) {
        begin_.node_ = begin_.node_->next_;
        delete begin_.node_->prev_;
        begin_.node_->prev_ = nullptr;
      } else {
        pos.node_->prev_->next_ = pos.node_->next_;
        pos.node_->next_->prev_ = pos.node_->prev_;
        delete pos.node_;
      }
      size_ -= 1;
    }
  }

  void push_back(const_reference value) { insert(end_, value); }

  void pop_back() {
    if (!empty()) {
      erase(iterator(end_.node_->prev_));
    }
  }

  void push_front(const_reference value) {
    if (empty()) {
      push_back(value);
    } else {
      insert(begin(), value);
    }
  }

  void pop_front() {
    if (!empty()) {
      erase(begin_);
    }
  }

  void swap(list &other) {
    auto temp = std::move(other);
    other = std::move(*this);
    *this = std::move(temp);
  }

  void merge(list &other) {
    if (size_ + other.size_ > max_size()) throw "Maximum of container";
    auto merged = list(size_ + other.size_);
    auto firstIter = begin();
    auto secondIter = other.begin();
    for (auto iter = merged.begin(); iter != merged.end(); ++iter) {
      if (firstIter == end()) {
        *iter = *secondIter;
        ++secondIter;
      } else {
        if (secondIter == other.end()) {
          *iter = *firstIter;
          ++firstIter;
        } else {
          if (*firstIter < *secondIter) {
            *iter = *firstIter;
            ++firstIter;
          } else {
            *iter = *secondIter;
            ++secondIter;
          }
        }
      }
    }
    clear();
    other.clear();
    *this = std::move(merged);
  }

  void splice(const_iterator pos, list &other) {
    if (size_ + other.size_ > max_size()) throw "Maximum of container";
    for (auto i = --(other.end()); i != other.begin(); --i) {
      insert(iterator(pos), *i);
    }
    insert(iterator(pos), *other.begin());
    other.clear();
  }

  void reverse() {
    auto leftIter = begin();
    auto rigthIter = iterator(end_.node_->prev_);
    for (size_type i = 0; i < size_ / 2; i += 1) {
      auto buf = *leftIter;
      *leftIter = *rigthIter;
      *rigthIter = buf;
      ++leftIter;
      --rigthIter;
    }
  }

  void unique() {
    auto i = begin();
    auto check = *i;
    ++i;
    while (i != end()) {
      if (*i == check) {
        erase(i);
      } else {
        check = *i;
      }
      ++i;
    }
  }

  template <typename... Args>
  iterator emplace(const_iterator ind, Args &&...args) {
    iterator iter = ind;
    for (auto &&item : {std::forward<Args>(args)...}) {
      push_back(++iter, item);
    }
    return iter;
  }

  template <typename... Args>
  void emplace_back(Args &&...args) {
    for (auto &&item : {std::forward<Args>(args)...}) {
      push_back(item);
    }
  }

  template <typename... Args>
  void emplace_front(Args &&...args) {
    for (auto &&item : {std::forward<Args>(args)...}) {
      push_front(item);
    }
  }

  void sort() {
    iterator middle = this->GetMiddleList();
    list<T> temp;
    for (auto it = temp.begin(); middle != this->end();) {
      iterator temp_it = middle;
      temp.insert(it, *middle);
      ++middle;
      this->erase(temp_it);
      ++it;
    }
    if (this->size() != 1) {
      this->sort();
    }
    if (temp.size() != 1) {
      temp.sort();
    }
    this->merge(temp);
  }

 private:
  iterator GetMiddleList() {
    auto it_fast = this->begin();
    auto it_slow = this->begin();
    auto it_fake = this->end();
    for (; it_fast != it_fake && ++it_fast != it_fake;) {
      ++it_slow;
      ++it_fast;
    }
    return it_slow;
  }

  iterator begin_, end_;
  size_type size_ = 0;
};
}  // namespace s21
#endif  // S21_CONTAINERS_S21_LIST_H
