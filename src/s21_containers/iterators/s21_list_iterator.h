#ifdef
#define

#include "../s21_list.h"

namespace s21 {
  template <typename T>
  class ListIterator {
    public:
      friend class List<T>;

      ListIterator(const List<T> &list) {
        if (list.begin_ == nullptr) {
          throw std::out_of_range("empty container");
        }
        node_ = list.begin_;
      }
      
      ~ListIterator() {
        delete node_;
      }

      friend ListIterator& operator++(ListIterator& other);
      friend ListIterator& operator--(ListIterator& other);

      const T& operator*() const {
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

    private:
      Node<T>* node_;
  };

  ListIterator& operator++(ListIterator& other) {
    if (other.node_->next_ != nullptr) { 
      other.node_ = other.node_->next_;
    }
    return other.node_;
  }

  ListIterator& operator--(ListIterator& other) {
    if (other.node_->prev_ != nullptr) { 
      other.node_ = other.node_->prev_;
    }
    return other.node_;
  }

}

#endif
