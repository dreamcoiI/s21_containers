#include "s21_list_iterator.h"
template <typename T>
s21::ListIterator<T>::ListIterator(const List<T> &list) {
  if (list.begin_ == nullptr) {
    throw std::out_of_range("empty container");
  }
  node_ = list.begin_;
}

s21::ListIterator& operator++(ListIterator& other) {
  if (other.node_->next != nullptr) { 
    other.node_ = other.node_->next;
  }
  return other.node_;
}
s21::ListIterator& operator--(ListIterator& other) {
  if (other.node_->prev != nullptr) { 
    other.node_ = other.node_->prev;
  }
  return other.node_;
}
template <typename T>
const s21::ListIterator::T& operator*(){
  if (node_ == nullptr) {
    throw std::out_of_range("node don't excist");
  }
  return node_->value;
}
bool s21::ListIterator::operator==(const ListIterator& other) const {
 if (this->node_ == nullptr || other.node_ == nullptr) {
  throw std:: out_of_range("node don't excist");
 }
 bool res = (this->node_ == other.node_)? true : false;
 return res;
}
bool s21::ListIterator::operator!=(const ListIterator& other) const {
 if (this->node_ == nullptr || other.node_ == nullptr) {
  throw std:: out_of_range("node don't excist");
 }
 bool res = (this->node_ != other.node_)? true : false;
 return res;
}
