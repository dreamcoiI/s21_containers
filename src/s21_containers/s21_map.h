#ifndef S21_CONTAINERS_S21_MAP_H_
#define S21_CONTAINERS_S21_MAP_H_

#include <stdexcept>

#include "s21_tree.h"

namespace s21 {
template <class Key, class Type>
class map {
 public:
  // Тим элемента-ключ
  using key_type = Key;
  // Значение элемента
  using mapped_type = Type;
  // пара ключ и значение
  using value_type = std::pair<Key, Type>;
  // Ссылка на элемент
  using reference = value_type &;
  // Константная ссылка на элемент
  using const_reference = const value_type &;

  // Компаратор, для словаря: Элементы считаются равными если значение их ключей
  // равны
  struct MapCmprt {
    bool operator()(const_reference op1, const_reference op2) const noexcept {
      return op1.first < op2.first;
    }
  };
  // Внутренние классы
  //  1)дерева
  using tree_type = RBTree<value_type, MapCmprt>;
  // 2)итератор
  using iterator = typename tree_type::iterator;
  // 3)константный итератор
  using const_iterator = typename tree_type::const_iterator;

  // Тип для размера контейнера
  using size_type = std::size_t;

  // конструктор создания(по-умолчанию)
  map() : tree_(new tree_type{}) {}

  // конструктор для создания инициализаторов(создает словарь(инициализирует с
  // помощью std::initializer_list))
  map(std::initializer_list<value_type> const &items) : map() {
    for (auto item : items) insert(item);
  }

  // конструктор копирования
  map(const map &m) : tree_(new tree_type(*m.tree_)) {}

  // конструктор переноса
  map(map &&m) noexcept : tree_(new tree_type(std::move(*m.tree_))) {}

  // оператор присваивания(копирование)
  map *operator=(const map &m) {
    *tree_ = *m.tree_;
    return *this;
  }

  // Деструктор
  ~map() {
    delete tree_;
    // так же зануляем tree_
    tree_ = nullptr;
  }

  // Доступ к указанному элементу(плюс проверка границ). Возвращает ссылку на
  // значение с ключом Если такого элемента нет-вызывается исключение
  //  std::out_of_range
  mapped_type &at(const key_type &key) {
    value_type search_pair(key, mapped_type{});
    iterator search_iter = tree_->Find(search_pair);

    if (search_iter == end())
      throw std::out_of_range("No elements with key");
    else
      return (*search_iter).second;
  }

  // const версия at()
  const mapped_type &at(const key_type *key) const {
    return const_cast<map<Key, Type> *>(this)->at(key);
  }

  // Возвращает ссылку на значение с ключом key. Если такого элемента нет,
  // то выполняется вставка
  mapped_type &operator[](const key_type &key) {
    value_type search_pair(key, mapped_type{});
    iterator search_iter = tree_->Find(search_pair);

    if (search_iter == end()) {
      std::pair<iterator, bool> res = tree_->UniqueInsert(search_pair);
      return (*res.first).second;
    } else
      return (*search_iter).second;
  }

  // возвращает итератор в начало контейнера
  iterator begin() noexcept { return tree_->begin_(); }

  // тот же begin только для const
  const_iterator begin() const noexcept { return tree_->begin_(); }

  // возвращает итератор на конец контейнера(после последнего элемента)
  iterator end() noexcept { return tree_->end_(); }

  // const версия для end()
  const_iterator end() const noexcept { return tree_->end_(); }

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
  void swap(map &other) noexcept { tree_->swap(*other.tree_); }

  // Вытаскиваем из other вставляем в контейнер. Если такой элемент есть-вставка
  // не происходит
  void merge(map &other) noexcept { tree_->UniqueMerge(*other->tree_); }

  // Проверка на элемент с ключом key(true-да,false-нет)
  bool contains(const key_type &key) const noexcept {
    value_type search_pair(key, mapped_type{});
    iterator search_it = tree_->Find(search_pair);
    return search_it != end();
  }

  // Вставка в контейнер элемента со значением value, если он (контейнер прим.)
  // еще не содержит такого элемента с таким же ключом
  std::pair<iterator, bool> insert(const value_type &value) {
    return tree_->UniqueInsert(value);
  }

  // Вставка в контейнер элемента со значениями key и obj, если он (контейнер
  // прим.) еще не содержит такого элемента с таким же ключом
  std::pair<iterator, bool> insert(const key_type &key,
                                   const mapped_type &obj) {
    return tree_->UniqueInsert(value_type{key, obj});
  }

  // Ищет по key элемент и вставляет его, если не нашел, вставляет новое
  // значение
  std::pair<iterator, bool> insert_or_assign(const key_type &key,
                                             const mapped_type &obj) {
    iterator res = tree_->Find(value_type{key, obj});

    if (res == end()) return tree_->UniqueInsert(value_type{key, obj});
    (*res).second = obj;

    return {res, false};
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

#endif  // S21_CONTAINERS_S21_MAP_H_
