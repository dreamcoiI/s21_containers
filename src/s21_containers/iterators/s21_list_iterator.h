#ifdef
#define

#include "../s21_list.h"

namespace s21 {
  template <typename T>
  class ListIterator {
    public:
      friend class List<T>;
      friend class ListConstIterator<T>;

      ListIterator(const List<T> &list);
      friend ListIterator& operator++(ListIterator& other);
      friend ListIterator& operator--(ListIterator& other);
      const T& operator*();
      bool operator==(const ListIterator& other) const;
      bool operator!=(const ListIterator& other) const;

    private:
      Node<T>* node_;
  };
}

#endif
