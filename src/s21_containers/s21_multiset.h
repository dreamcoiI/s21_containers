#ifndef S21_CONTAINERSPLUS_S21_MULTISET_H
#define S21_CONTAINERSPLUS_S21_MULTISET_H

#include "s21_tree.h"

namespace s21 {
    template<class Key>
    class multiset {
        //Тим элемента-ключ
        using key_type = Key;
        //Значение элемента
        using value_type = key_type;
        //Ссылка на элемент
        using reference = value_type &;
        //Константная ссылка на элемент
        using const_reference = const value_type &;
        //Тип для размера контейнера
        using size_type = std::size_t;

        //Внутренние классы
        // 1)дерева
        using tree_type = RBTree<value_type>;
        // 2)итератор
        using iterator = typename tree_type::iterator;
        // 3)константный итератор
        using const_iterator = typename tree_type::const_iterator;

        //конструктор по умолчанию, создает пустое множество
        multiset() : tree_(new tree_type{}){}

        //конструктор создания множества(инициализация с помощью
        // std::initializer_list)
        multiset(std::initializer_list<value_type> const &items) : multiset() {
            for (auto item : items) {
                insert(item);
            }
        }

        //конструктор копирования
        multiset(const multiset &m) : tree_(new tree_type(*m.tree_)) {}

        //оператор присваивания копированием
        multiset &operator=(const multiset &m) {
            *tree_ = *m.tree_;
            return *this;
        }

        //конструктор переноса
        multiset(multiset &&m) noexcept : tree_(new tree_type(std::move(*m.tree_))) {}

        //оператор присваивания переносом
        multiset() &operator=(multiset() &&m) noexcept {
            *tree_ = std::move(*m.tree_);
            return *this;
        }

        //Деструктор
        ~multiset() {
            delete tree_;
            //не забываем занулить
            tree_ = nullptr;
        }

        //возвращает итератор в начало контейнера
        iterator begin() noexcept { return tree_->begin_(); }

        //тот же begin только для const
        const_iterator begin() const noexcept { return tree_->begin_(); }

        //возвращает итератор на конец контейнера(после последнего элемента)
        iterator end() noexcept { return tree_->end_(); }

        // const версия для end()
        const_iterator end() const noexcept { return tree_->end_(); }



    private:
        tree_type *tree_;
    };
} //namespace

#endif S21_CONTAINERSPLUS_S21_MULTISET_H