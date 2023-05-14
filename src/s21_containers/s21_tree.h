#ifndef S21_CONTAINERS_SRC_S21_CONTAINERS_S21_TREE_H
#define S21_CONTAINERS_SRC_S21_CONTAINERS_S21_TREE_H

#include <functional>
#include "s21_vector.h"
#include <limits>

namespace s21 {

    enum RBTreeColor {
        tRed,
        tBlack
    };

    template <typename  key, typename Comparator = std::less<key>>
            class RBTree {
            private:
                struct RedBlackNode;
                struct RedBlackIterator;
                struct RedBlackIteratorConst;
            public:
                using key_type = key;
                using const_reference = const key_type &;
                using reference = key_type &;
                using const_iterator = const RedBlackIteratorConst;
                using iterator = RedBlackIterator;
                using size_type = std::size_t;

                using tree_type = RBTree;
                using tree_node = RedBlackNode;
                using tree_color = RBTreeColor;


                //создание пустого дерева, конструктор по умолчанию
                RBTree() :head_(new tree_node), size_(0U){

                }

                //конструктор переноса
                RBTree(const tree_type&other) :RBTree() {
                    if(other.size_()> 0) {
                        copyFromOther(other);
                    }
                }
                //конструктор переноса, обмен данными с other
                RBTree(tree_type &&other) noexcept: RBTree() {
                    swap(other);
                }

                //Присваивание копированием
                tree_type &operator=(const tree_type &other) {
                    if(this != other) {
                        if(other._size_()>0) {
                            copyFromOther(other);
                        } else {
                            clear();
                        }
                    }
                }
                //Присваивание переносом
                tree_type &operator=(tree_type &&other) noexcept {
                    clear();
                    swap(other);
                    return *this;
                }
                //деструктор
                ~RBTree() {
                    clear();
                    delete head_;
                    //делаем указатель чтобы избежать дальнейшие сбои
                    head_= nullptr;
                }
                //возвращает кол-во эл-ов в контейнере
                size_type _size_() const noexcept {
                    return size_;
                }

                reference operator *() const noexcept{
                    return node_->key_;
                }

                void swap(tree_type &other) noexcept {
                    std::swap(head_,other.head_);
                    std::swap(size_,other.size_);
                    std::swap(cmprt,other.cmprt);
                }
                //Удаляет содержимое контейнера
                void clear() noexcept{
                    destroy(Root());
                    initializerHead();
                    size_=0;
                }

                //функция нужна для проверки пустой ли контейнер, 1 -да, 0 - нет
                bool isEmpty() const noexcept {
                    return size_==0;
                }

                private:

                tree_node *head_;

                tree_node *_parent_;

                tree_node *_left_;

                tree_node *_right_;

                tree_node *_key_;

                tree_node _color_;

                Comparator cmprt;

                size_type size_{};


                void destroy(tree_node *node) noexcept{
                    if(node == nullptr) return;
                    destroy(node->_left_);
                    destroy(node->_right_);
                    delete node;
                }


                //Инициализация узла head
                void initializerHead() noexcept{
                    Root() = nullptr;
                    MostLeft()=head_;
                    MostRight() = head_;
                }

                tree_node *&Root() {
                    return head_->_parent_;
                }
                const tree_node *&Root() const {
                    return head_->_parent_;
                }

                tree_node *MostLeft() {
                    return head_->_left_;
                }

                const tree_node *MostLeft() const {
                    return head_->_left_;
                }

                tree_node *MostRight() {
                    return head_->_right_;
                }

                const tree_node *MostRight() const {
                    return head->_right_;
                }

                [[nodiscard]] tree_node  *copytree(const tree_node *node, tree_node *_parent_) {
                    auto *tmp = new tree_node {node->_key_, node->_color_};
                    try {
                        if(node->_left_)
                            tmp -> _left_ = copytree(node->_left_, tmp);
                        if(node->_right)
                            tmp->right = copytree(node->_left_,tmp);
                    } catch (...) {
                        Destroy(tmp);
                        throw;
                    }

                    tmp->_parrent_ = parent;
                    return tmp;
                }

                tree_node *MinimumSearch(tree_node *node)const noexcept{
                    while (node->_left_ != nullptr) {
                        node=node->_left_;
                    }
                    return node;
                }

                tree_node *MaximumSearch(tree_node *node)const noexcept{
                    while (node->_right_ != nullptr) {
                        node = node->_right_;
                    }
                    return node;
                }

                void copyFromOther(const tree_type &other) {
                    tree_node *tmp_copy_root = copytree(other.Root(), nullptr);
                    clear();
                    Root()=tmp_copy_root;
                    Root()->_parent_=head_;
                    MostLeft()= MinimumSearch(Root());
                    MostRight() = MaximumSearch(Root());
                    size_=other.size_;
                    cmprt=other.cmprt;
                }
            };

}

#endif //S21_CONTAINERS_SRC_S21_CONTAINERS_S21_TREE_H