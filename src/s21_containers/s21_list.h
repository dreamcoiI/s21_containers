#ifndef S21_CONTAINERS_S21_LIST_H
#define S21_CONTAINERS_S21_LIST_H

#include "iterators/s21_list_iterator.h"

namespace s21 {
  template <typename T>
  struct Node {
    T value_;
    Node* next_;
    Node* prev_;
    Node() : value_(), next_(nullptr), prev_(nullptr) {}
    Node(const T& value, const Node* prev, const Node* next)
        : value_(value), next_(next), prev_(prev) {}
    Node(T&& value)
        : value_(std::move(value)), next_(nullptr), prev_(nullptr) {}
  };

  template <typename T>
  class List {
    public:
      friend class ListIterator<T>;
      using value_type = T;
      using reference = T&;
      using const_reference = const T&;
      using iterator = ListIterator<T>;
      using const_iterator = const ListIterator<T>;
      using size_type = size_t;

      List(): end_(new ListNode(value_type())) {}

      List(size_type n): List() {
        for (size_type i = 0; i < n; i += 1) {
          push_back(value_type());
        }
      }

      List(std::initializer_list<value_type> const &items): list() {
        for (const auto& i : items) {
          push_back(i);
        }
      }

      List(const List &l): list() { operator=(l); }

      List(List &&l): list() {
        if (this != &l) {
          clear();
          size_ = l.size_;
          begin_ = l.begin_;
          end_.node_->prev_ = l.end_.node_->prev_;
          if (end_.node_->prev_ != nullptr) {
            end_.node_->prev_->next_ = end_.node_;
          }
          l.size_ = 0;
          l.end_.node_.prev_ = nullptr;
        }
      }

      ~List() {
        clear();
        delete end_;
      }

      operator=(const List &l) {
        if (this != &l) {
          clear();
          for (const_iterator it = l.begin(); it != l.end(); ++it) {
            push_back(*it);
          }
        }
        return *this;
      }

      reference front() {
        if (begin_ == nullptr) {
          throw "Collection is empty";
        }
        return &(begin_.node_->value_);
      }

      reference back() {
        if (begin_ == nullptr) {
          throw "Collection is empty";
        }
        return &(end_.node_->prev_->value_);
      }

      iterator begin() const {
        if (empty()) {
          return end_;
        } else {
          return begin_;
        }
      }

      iterator end() const {
        return end_;
      }

      bool empty() const noexcept {
        return size_ == 0;
      }

      size_type size() const {
        return size_;
      }

      size_type max_size() const noexcept {
        return (std::numeric_limits<size_type>::max() / sizeof(ListNode<T>) / 2);
      }

      void clear() {
        while (!empty()) {
          pop_back();
        }
        end_.node_->prev_ == nullptr;
      }

      iterator insert(iterator pos, const_reference value) {
        if (size_ + 1 > max_size()) throw "Maximum of container";
        iterator res();
        if (pos == end_) {
          if (end_.node_->prev_ == nullptr) {
            begin_.node_ = new Node<value_type>(value, nullptr, end_.node_);
            end_.node_->prev_ = begin_.node_;
            res = begin_;
          } else {
            end_.node_->prev_->next_ = new Node<value_type>(value, end_.node_->prev_, end_.node_);
            res = iterator(end_.node_->prev_);
          }
        } else {
          if (pos == begin_) {
            begin_.node_ = new Node<value_type>(value, nullptr, pos.node_);
            pos.node_->prev_ = begin_.node_;
            res = begin_;
          } else {
            pos.node_->prev_->next_ = new Node<value_type>(value, pos.node_->prev_, pos.node_);
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
            begin_ = begin_.node_->next_;
            delete begin_.node_->prev_;
            begin_.node_->prev_ = nullptr;
          } else {
            pos.node_->prev_->next_ = pos.node_->next_;
            pos.node_->next_->prev_ = pos.node_->prev_;
            delete pos;
          }
          size_ -= 1;
        }
      }

      void push_back(const_reference value) {
        insert(end_ ,value);
      }

      void pop_back() {
        if(!empty()) {
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

      void swap(list& other) {
        auto temp = std::move(other);
        other = std::move(*this);
        *this = std::move(temp);
      }

      void merge(list& other) {
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

      void splice(const_iterator pos, list& other) {
        if (size_ + other.size_ > max_size()) throw "Maximum of container";
        for (auto i = --(other.end()); i != other.begin(); --i) {
          insert(iterator(pos), *i);
        }
        insert(iterator(pos), *other.begin());
        other.clear();
      }

      void reverse() {
        auto left = begin_;
        auto right = iterator(end_.node_->prev_);
        if (left != right) {
          for (auto i = left; i != end_; ++i) {
            auto buf = i.node_->next_;
            i.node_->next_ = left.node_->prev_;
            i.node_->prev_ = buf;
          }
          begin_ = right;
          begin_.node_->prev_ = nullptr;
          end_.node_->prev_ = left.node_;
          left.node_->next = end_.node_;
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
      template <typename ... Args>
      iterator emplace (const_iterator ind, Args &&...args) {
        iterator iter = ind;
        for(auto &&item: {std::forward<Args>(args)...}) {
          push_back(++iter, item);
        }  
        return iter;
      }

      template <typename... Args>
      void emplace_back(Args &&...args) {
        for(auto &&item: {std::forward<Args>(args)...}) {
          push_back(item);
        }      
      }

      template <typename... Args>
      void emplace_front(Args &&...args) {
        for(auto &&item: {std::forward<Args>(args)...}) {
          push_front(item);
        }      
      }

      void sort() {
        if (size_ > 1) {
          auto left = begin_;
          auto right = --end_;
          this->newSort_(left, right;
        }
      }


    private:
      void newSort_(iterator left, iterator right, size_type listSize) {
        if (last_element != this->begin() - 1 && first_element != last_element &&
            first_element != last_element + 1) {
          iterator iter = this->swapSort_(first_element, last_element);
          this->newSort_(first_element, iter - 1);
          this->newSort_(iter + 1, last_element);
        }
      }


      iterator swapSort_(iterator first, iterator last) {
        value_type x = *last;
        iterator i = --first;
        for (iterator j = first; j != last; ++j) {
          if (*j < x) {
            i = i + 1;
            std::swap(*i, *j);
          }
        }
        i = i + 1;
        std::swap(*i, *last_element);
        return i;
      }

      iterator begin_, end_;
      size_type size_;
  };
}
#endif //S21_CONTAINERS_S21_LIST_H
