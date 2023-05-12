#ifndef S21_CONTAINERS_ITERATORS_S21_LIST_ITERATOR_H
#define S21_CONTAINERS_ITERATORS_S21_LIST_ITERATOR_H

#include "../s21_list_node.h"

namespace s21 {


  template <typename T>
  class ListIterator {
    public:
      friend class list<T>;

      ListIterator() {}

      ListIterator(list<T> list) {
        if (list.begin_ == nullptr) {
          throw std::out_of_range("empty container");
        }
        node_ = list.begin_;
      }

      ListIterator(Node<T> *node) {
        if (node == nullptr) {
          throw std::out_of_range("empty container");
        }
        node_ = node;
      }    

      ~ListIterator() {
        delete node_;
      }

      ListIterator& operator++() {
        if (node_->next_ != nullptr) { 
          node_ = node_->next_;
        }
        return *this;
      }

      ListIterator& operator--() {
        if (node_->prev_ != nullptr) { 
          node_ = node_->prev_;
        }
        return *this;
      }

      T& operator*() const {
        if (node_ == nullptr) {
          throw std::out_of_range("node don't excist");
        }
        return node_->value_;
      }

      bool operator==(const ListIterator& other) const {
        if (this->node_ == nullptr || other.node_ == nullptr) {
          throw std:: out_of_range("node don't excist");
        }
        bool res = (this->node_ == other.node_)? true : false;
        return res;
      }

      bool operator!=(const ListIterator& other) const {
        if (this->node_ == nullptr || other.node_ == nullptr) {
          throw std:: out_of_range("node don't excist");
        }
        bool res = (this->node_ != other.node_)? true : false;
        return res;
      }

      class ConstIterator {
        public:
        Node<T>* node;

        ConstIterator(): ListIterator() {}

        ConstIterator(Node<T> *node): ListIterator(node) {}

        ConstIterator operator=(const ListIterator &other) {
          this->node = other.node_;
          return *this;
        }

        ConstIterator& operator++() {
          if (node->next_ != nullptr) { 
            node = node->next_;
          }
          return *this;
        }

        ConstIterator& operator--() {
          if (node->prev_ != nullptr) { 
            node = node->prev_;
          }
          return *this;
        }

        const T& operator*() const {
          if (node == nullptr) {
            throw std::out_of_range("node don't excist");
          }
          return node->value_;
        }

        bool operator!=(const ConstIterator& other) const {
          if (this->node == nullptr || other.node == nullptr) {
            throw std:: out_of_range("node don't excist");
          }
          bool res = (this->node != other.node)? true : false;
          return res;
        }

        bool operator==(const ConstIterator& other) const {
          if (this->node == nullptr || other.node == nullptr) {
            throw std:: out_of_range("node don't excist");
          }
          bool res = (this->node == other.node)? true : false;
          return res;
        }

      };
    private:
      Node<T>* node_;
  };

}

#endif //S21_CONTAINERS_ITERATORS_S21_LIST_ITERATOR_H
