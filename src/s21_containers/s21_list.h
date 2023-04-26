#ifndef
#define

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
      friend class ListConstIterator<T>;
      using value_type = T;
      using reference = T&;
      using const_reference = const T&;
      using iterator = ListIterator<T>;
      using const_iterator = ListConstIterator<T>;
      using size_type = size_t;

      List();
      List(size_type n);
      List(std::initializer_list<value_type> const &items);
      List(const list &l);
      List(list &&l);
      ~List();
      operator=(const List &l);

      const_reference front();
      const_reference back();
      iterator begin();
      iterator end();

      bool empty() const;
      size_type size();
      size_type max_size();

      void clear();
      iterator insert(iterator pos, const_reference value);
      void erase(iterator pos);
      void push_back(const_reference value);
      void pop_back();
      void push_front(const_reference value);
      void pop_front();
      void swap(list& other);
      void merge(list& other);
      void splice(const_iterator pos, list& other);
      void reverse();
      void unique();
      void sort();

    private:
      iterator begin_, end_;
      size_type size_;
  };
}
#endif
