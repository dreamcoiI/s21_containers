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
                    //делаем указатель, чтобы избежать дальнейшие сбои
                    head_= nullptr;
                }
                //возвращает кол-во эл-ов в контейнере
                size_type _size_() const noexcept {
                    return size_;
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

                //Возвращает максимальное допустимое кол-во эл-ов в контейнере
                //Так как gcc ограничивает объекты размером в половину адресного пространства, делим на 2,
                //Так как в дереве хранится указатель на head(голову), кол-во созданных эл-ов(size_),
                // и компаратор (cmprt)- это все вместе получается tree_type, а так же выделяется память
                // на один служебный узел tree_node(он есть даже у пустого дерева), мы вычитаем эти значения
                //для получения максимальной допустимой памяти
                // А для нахождения максимального кол-ва эл-ов мы делим полученное значение на sizeof одного узла

                size_type maxSize() const noexcept {
                    return ((std::numeric_limits<size_type>::max()/2) -
                            sizeof (tree_type)-sizeof (tree_node))/sizeof (tree_node);
                }

                //возвращает итератор на начало контейнера
                iterator begin_() noexcept {
                    return iterator (MostLeft());
                }

                //конст begin
                const_iterator begin_() const noexcept {
                    return const_iterator (MostLeft());
                }

                //возвращает итератор на конец контейнера(после последнего эл-та)
                iterator end_() noexcept {
                    return iterator (head_);
                }

                //конст end
                const_iterator end_() const noexcept {
                    return const_iterator (head_);
                }

                //итератор для вставки в контейнер элемента в контейнер, если есть уже такой ключ
                //вставка производится по верхней границе диапазона
                iterator InsertKey(const key_type &key) {
                    auto *new_tmp = new tree_node{key};
                    return InsertKey(Root(),new_tmp,false).first;
                }

                //Мерджим элементы из other в this
                void merge_(tree_type &other) {
                    if(this!= &other) {
                        iterator anotherBegin = other.begin_();
                        while (other.size_ > 0) {
                            tree_node *mvgNode = other.begin_();
                            ++anotherBegin;
                            if(mvgNode->_left_!= nullptr)
                                mvgNode->_left_->_parent_=mvgNode->_parent_;
                            if(mvgNode->_right_ != nullptr)
                                mvgNode->_right_->_parent_=mvgNode->_parent_;
                            if(mvgNode->_parent_->_left_ == mvgNode)
                                mvgNode->_parent_->_left_ = nullptr;
                            if(mvgNode->_parent_->_right_ == mvgNode)
                                mvgNode->_parent_->_right_ = nullptr;

                            mvgNode->toDefaultNode();
                            --other.size_;

                        }
                    }
                }
                private:

                tree_node *head_;

                tree_node *parent_;

                tree_node *left_;

                tree_node *right_;

                key_type *key_;

                tree_color color_;

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
                    return head_->_right_;
                }

                [[nodiscard]] tree_node  *copytree(const tree_node *node, tree_node *parent_) {
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

                    tmp->_parrent_ = parent_;
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

                void toDefaultNode() noexcept {
                    left_ = nullptr;
                    right_= nullptr;
                    parent_= nullptr;
                    color_=tRed;
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

                std::pair<iterator,bool>InsertKey(tree_node *head, tree_node *root, bool uniq) {
                    tree_node *tmp = head;
                    tree_node *parent = nullptr;

                    while (tmp != nullptr) {
                        parent = tmp;
                        if(cmprt(root->key,head->key_))
                            head=head->left_;
                        else {
                            if(uniq == false) tmp = tmp->right_;
                            else return {iterator(head), false};
                        }
                    }
                    if(parent!= nullptr) {
                        root->parent_=parent;
                        if(cmprt(root->key_,parent->key)) parent->left_=root;
                        else parent->right_=root;
                    } else {
                        root->color_ = tBlack;
                        root->parent_= head_;
                        Root()=root;
                    }
                    ++size_;
                    if(MostLeft()==head_||MostLeft()->left_!= nullptr) MostLeft() = root;
                    if(MostRight()==head_||MostRight()->right_ != nullptr) MostRight() = root;
                    BalancingInsertTree();
                    return  {iterator(root), true};

                }

                //Для балансировки дерева нужно знать несколько правил:
                //1) Каждый узел промаркирован красным или чёрным цветом
                //2) Корень и конечные узлы (листья) дерева - чёрные
                //3) У красного узла родительский узел - черный
                //4)Все простые пути из любого узла х до листьев содержат одинаковое количества чёрных узлов
                //5) Чёрный узел может иметь чёрного родителя
                // В интернете много статей где объясняется как нужно делать балансировку.
                // Вот две из них в которых приводится алгоритм:
                // https://fkti5301.github.io/exam_tickets_aisd_2017_kolinko/tickets/12.html
                // https://habr.com/ru/companies/otus/articles/472040/
                //Соответственно, для балансировки дерева нам понадобятся функции вращения
                void BalancingInsertTree(tree_node *node) {
                    //Папа
                    tree_node *father = node->parent_;

                    while(node!= Root() && father->color_ == tRed) {
                        //Дед
                        tree_node *grandpa = father->parent_;
                        if(grandpa->left != father) {
                            //Ситуация когда "дядя" слева
                            tree_node  *uncle = grandpa->left_;
                            if(uncle != nullptr && uncle->color_ == tRed) {
                                //если дядя и папа красные, мы меняем цвет у них, деду ставим красный
                                father->color_ = tBlack;
                                uncle->color_=tBlack;
                                grandpa->color_=tRed;

                                node=grandpa;
                                father=node->parent_;
                            } else {
                                //если дядя черный, папа и дед в разных сторонах дерева
                                if(father->left_==node) {
                                    //функция поворота направо
                                    RightRotate(father);
                                    std::swap(father,node);
                                }
                                //если дядя черный, а папа и дед в одной стороне
                                //Функция поворота налево
                                LeftRotate(grandpa);
                                father->color_=tBlack;
                                grandpa->color_=tRed;
                                break;
                            }
                        } else {
                            //ситуация когда дядя справа
                            tree_node* uncle = grandpa->right_ ;
                            if(uncle!= nullptr && uncle->color_ == tRed) {
                                father->color_=tBlack;
                                uncle->color_=tBlack;
                                grandpa->color_=tRed;

                                node=grandpa;
                                father=node->parent_;
                            } else {
                                //дядя черный папа и дед в разных сторонах
                                if(father->right_==node) {
                                    //Функция поворота налево
                                    LeftRotate(father);
                                    std::swap(father,node);
                                }
                                //дядя черный папа и дед в одной стороне
                                //Функция поворота направо
                                RightRotate(grandpa);
                                grandpa->color=tRed;
                                father->color_=tBlack;
                                break;
                            }
                        }
                    }
                    //Корень всегда черный!
                    Root()->color_=tBlack;
                }

                //функция поворота налево
                void LeftRotate(tree_node *node) noexcept  {
                    //так как поворот налево, опорный узел будет правым.
                    tree_node *const support=node->right_;
                    support->parent_=node->parent_;

                    if(node==Root()) {
                        //если у нас нода была корнем, то опорный узел становится корнем
                        Root()=support;
                    } else if(node->parent_->left_==node) {
                        //если узел у родителя был слева, то опорным становится левый узел от родителя
                        node->parent_->left_=support;
                    } else {
                        //если узел у родителя был справа, то опорным становится правый узел от родителя
                        node->parent_->right=support;
                    }

                    node->right_=support->left_;
                    if(support->left_ != nullptr)
                        support->left_->parent_ = node;
                    node->parent_ = support;
                    support->left_=node;
                }

                //функция поворота направо (делаем все аналогично как с поворотом налево(только направо)
                void RightRotate(tree_node *node) noexcept {
                    //по аналогии с поворотом налево берем опорны узел(опорным будет узел слева!)
                    tree_node *const support = node->left_;
                    support->parent_=node->parent_;

                    if(node==Root())
                        Root()=support;
                    else if (node->parent_->right_ == node)
                        node->parent_->right_=support;
                    else
                        node->parent_->left_=support;

                    node->left_=support->right;
                    if(support->right_ != nullptr)
                        support->right_->parent_=node;
                    node->parent_=support;
                    support->right_=node;
                }


                //можно много расписывать про извлечение узла дерева по определенной позиции,
                //есть несколько статей, которые помогали мне при написании данной функции
                //https://habr.com/ru/companies/otus/articles/521034/
                //https://www.geeksforgeeks.org/deletion-in-red-black-tree/
                //Далее будут рассмотрены случаи которые описаны в этих статьях(К(номер)-красный,Ч(номер)-черный)
                tree_node* ExtractionNode(iterator ind) noexcept{
                    if(ind == end_()) {
                        return nullptr;
                    }

                    tree_node *removing_node=ind.node_;
                    //Когда у нас либо к2 или ч2(смотреть первую ссылку)
                    if(removing_node->left != nullptr && removing_node->right_ != nullptr) {
                        //находим самую левый узел в правой части(мин справа)
                        tree_node *tmp = MinimumSearch(removing_node->right_);
                        //нужна функция для свапа удаляемого и временного(заменяемого) узла
                    }
                    //Так же в первой ссылке было известно, что случай когда К1-невозможен(нарушает балансировку дерева)
                    //Рассмотрим случай Ч1
                    if  (removing_node->color_==tBlack&&(
                            (removing_node->left_ != nullptr && removing_node->right_ == nullptr)||
                            (removing_node->left_ == nullptr && removing_node->right_ != nullptr)
                            ))
                    {
                        tree_node * tmp;
                        if (removing_node->left_ != nullptr)
                            tmp=removing_node->left_;
                        else
                            tmp=removing_node->left_;
                        //нужна функция для свапа удаляемого и временного(заменяемого) узла
                    }
                    //обработки К0 и Ч0(так как с К0 нам никаких дополнительных обработок)
                    //Ч0 самый сложный, так как:
                    //После удаления чёрного элемента изменяется чёрная высота поддерева и
                    // нужно выполнить балансировку для его родительского элемента.
                    //Всего нужно рассмотреть 6 разных случаев:
                    //
                    //КЧ1 и КЧ2 – родитель красный, левый ребёнок чёрный.
                    //ЧК3 и ЧК4 – родитель чёрный, левый ребёнок красный.
                    //ЧЧ5 и ЧЧ6 – родитель чёрный, левый ребёнок чёрный.
                    if (removing_node->color_ == tBlack &&
                    removing_node->left_ == nullptr && removing_node->right_ == nullptr) {
                        //нужна отдельная функция, так как писать тут очень много, будет не читабельно
                    }
                    //теперь мы изымаем ноду, которую искали
                    if(removing_node == Root())
                        //в данном случае мы удаляем корень дерева(возможно когда единственный элемент дерева)
                        //делаем отдельную обработку, для остальных случаев будет описано ниже
                        initializerHead();
                    else {
                        //тут мы находим, где находится узел и отцепляем его от родителя
                        if(removing_node==removing_node->parent_->left_)
                            removing_node->parent_->left_ = nullptr;
                        else
                            removing_node->parent_->right_ = nullptr;

                        //ищем новые максимум и минимум для узла(только в случае если мы удали предыдущие(й))
                        if(MostRight() == removing_node)
                            MostRight() = MaximumSearch(Root());
                        if(MostLeft() == removing_node)
                            MostLeft() = MinimumSearch(Root());

                    }

                    //не забываем уменьшить размер дерева на 1 узел(так как мы удалили элемент)
                    --size_;

                    //ну и теперь приводим дерево к стандартному состоянию

                    removing_node->toDefaultNode();
                    return  removing_node;
                }
                //Нужна функция извлечения узла из дерева по передаваемой позиции



                reference operator *() const noexcept{
                    return node_->key_;
                }
            };
}

#endif //S21_CONTAINERS_SRC_S21_CONTAINERS_S21_TREE_H