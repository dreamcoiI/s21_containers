#ifndef S21_CONTAINERS_S21_SET_H_
#define S21_CONTAINERS_S21_SET_H_

#include <vector>

#include "s21_tree.h"

namespace s21 {
template <class Key>
class set {
 public:
  // Тим элемента-ключ
  using key_type = Key;
  // Значение элемента
  using value_type = key_type;
  // Ссылка на элемент
  using reference = value_type &;
  // Константная ссылка на элемент
  using const_reference = const value_type &;
  // Тип для размера контейнера
  using size_type = std::size_t;

  // Внутренние классы
  //  1)дерева
  using tree_type = RBTree<value_type>;
  // 2)итератор
  using iterator = typename tree_type::iterator;
  // 3)константный итератор
  using const_iterator = typename tree_type::const_iterator;

  // конструктор по умолчанию, создает пустое множество
  set() : tree_(new tree_type{}) {}

  // конструктор создания множества(инициализация с помощью
  //  std::initializer_list)
  set(std::initializer_list<value_type> const &items) : set() {
    for (auto item : items) {
      insert(item);
    }
  }

  // конструктор копирования
  set(const set &s) : tree_(new tree_type(*s.tree_)) {}

  // оператор присваивания копированием
  set &operator=(const set &s) {
    *tree_ = *s.tree_;
    return *this;
  }

  // конструктор переноса
  set(set &&s) noexcept : tree_(new tree_type(std::move(*s.tree_))) {}

  // оператор присваивания переносом
  set &operator=(set &&s) noexcept {
    *tree_ = std::move(*s.tree_);
    return *this;
  }

  // Деструктор
  ~set() {
    delete tree_;
    // не забываем занулить
    tree_ = nullptr;
  }

  // возвращает итератор в начало контейнера
  iterator begin() noexcept { return tree_->begin_(); }

  // тот же begin только для const
  const_iterator begin() const noexcept { return tree_->begin_(); }

  // возвращает итератор на конец контейнера(после последнего элемента)
  iterator end() noexcept { return tree_->end_(); }

  // const версия для end()
  const_iterator end() const noexcept { return tree_->end_(); }

  // Находит элемент с ключом эквивалентный ключу key
  iterator find(const key_type &key) noexcept { return tree_->Find(key); }

  // const версия для find()
  const_iterator find(const key_type &key) const noexcept {
    return tree_->Find(key);
  }

  // Возвращает кол-во элементов контейнера
  size_type size() const noexcept { return tree_->_size_(); }

  // Возвращает true если контейнер пустой, false если нет
  bool empty() const noexcept { return tree_->isEmpty(); }

  // Возвращает максимально допустимое кол-во элементов в контейнере
  size_type max_size() const noexcept { return tree_->maxSize(); }

  // очистка содержимого контейнера
  void clear() noexcept { tree_->clear(); }

  // удаляет элемент по передаваемой позиции ind
  void erase(iterator pos) noexcept { tree_->Erase(pos); }

  // Обменивает содержимое контейнера с other
  void swap(set &other) noexcept { tree_->swap(*other.tree_); }

  // Вытаскиваем из other вставляем в контейнер. Если такой элемент есть-вставка
  // не происходит
  void merge(set &other) noexcept { tree_->UniqueMerge(*other.tree_); }

  // Вставка элемента в контейнер, если такого ключа в контейнере нет
  std::pair<iterator, bool> insert(const value_type &value) {
    return tree_->UniqueInsert(value);
  }

  // Проверка на элемент с ключом key(true-да,false-нет)
  bool contains(const key_type &key) const noexcept {
    return tree_->Find(key) != tree_->end_();
  }

  // Размещаем новые элементы в контейнер, если такого ключа еще нет
  template <typename... Args>
  std::vector<std::pair<iterator, bool>> emplace(Args &&...args) {
    return tree_->UniqEmplace(std::forward<Args>(args)...);
  }

 private:
  tree_type *tree_;
};
}  // namespace s21

#endif  // S21_CONTAINERS_S21_SET_H_