#include "s21_list.h"
s21::List<T>::List() {
/*  Node<T>* elem;
  begin_ = elem;
  end_ = elem->next;*/
  size_ = 1;
}
s21::List<T>::List(size_type n): List() {
  for (size_type i = 0; i < n; i += 1) {
    push_back(value_type());
    size_++;
  }
}
s21::List<T>::List(std::initializer_list<value_type> const &items) {
}
s21::List<T>::List(const list &l) {
}
s21::List<T>::List(list &&l) {
}
s21::List<T>::~List() {
}

operator=(const List &l);

const_reference front();
const_reference back();
iterator begin();
iterator end();

bool empty() const {
  return size_ == 0;
}

size_type size();
size_type max_size();

void clear();
iterator s21::List::insert(iterator pos, const_reference value) { //akkuratno pochekat'
  iterator res();
  if (pos.node == end_) {
    if (end_->prev_ == nullptr) {
      begin_.node_ = new Node<value_type>(value, nullptr, end_.node_);
      end_->prev_ = begin_.node_;
      res = iterator(begin_);
    } else {
      end_->prev_->next_ = new Node<value_type>(value, end_.node_->prev_, end_.node_);
      res = iterator(end_.node_->prev_);
    }
  } else {
    if (pos.node_->prev_ == nullptr) {
      begin_.node_ = new Node<value_type>(value, nullptr, pos.node_);
      pos.node_->prev_ = begin_.node_;
      res = iterator(begin_);
    } else {
      pos.node_->prev_->next_ = new Node<value_type>(value, pos.node_->prev_, pos.node_);
      pos.node_->prev_ = pos.node_->prev_->next_;
      res = iterator(pos.node_->prev_);
    }
  }
  size_ += 1;
  return res;
}
void erase(iterator pos);
void s21::List::push_back(const_reference value) {
  insert(end_ ,value);
}
void pop_back();
void push_front(const_reference value) {
  if (empty()) {
    push_back(value);
  } else {
    insert(begin(), value);
  }
}
  void pop_front();
  void swap(list& other);
  void merge(list& other);
  void splice(const_iterator pos, list& other);
  void reverse();
  void unique();
  void sort();

