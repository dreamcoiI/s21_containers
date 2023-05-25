#ifndef S21_CONTAINERS_SRC_S21_CONTAINERS_S21_TREE_H
#define S21_CONTAINERS_SRC_S21_CONTAINERS_S21_TREE_H

#include <functional>
#include "s21_vector.h"
#include <limits>
#include <vector>

namespace s21 {

    enum RBTreeColor {
        tRed,
        tBlack
    };

    template <typename  Key, typename Comparator = std::less<Key>>
            class RBTree {
            private:
                struct RedBlackNode;
                struct RedBlackIterator;
                struct RedBlackIteratorConst;
            public:
                using key_type = Key;
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
                            InsertKey(Root(),mvgNode,false);
                        }
                        other.initializerHead();
                    }
                }

                //Извлекаем из other и вставляем в this,
                // но если в this уже есть ключ который равен ключу в other, то ничего не извлечется
                void UniqueMerge(tree_type &other) {
                    if(this != &other) {
                        iterator o_begin = other.begin_();
                        iterator o_end = other.end_();

                        while (o_begin != o_end) {
                            iterator res = Find(o_begin.node_->key_);
                            if(res == end_()) {
                                iterator  tmp = o_begin;
                                ++o_begin;
                                tree_node *mvg_node = other.ExtractionNode(tmp);
                                InsertKey(Root(),mvg_node, false);
                            } else {
                                ++o_begin;
                            }
                        }
                    }
                }

                //функция для поиска элемента с ключом key
                iterator Find(const_reference key) {
                    iterator res = LowBow(key);
                    if(res == end_() || cmprt(key,*res))
                        //Если нижняя граница не нашлась, или нашел элемент > key
                        return end_();
                    //в остальных возвращаем результат работы функции LowBow
                    return res;
                }


                //а данная функция нужна для поиска минимального элемента который не меньше key
                iterator LowBow(const_reference key) {
                    //начнем искать с корня
                    tree_node *begin = Root();
                    //Если ничего не найдем, то используется значение по-умолчанию(end)
                    tree_node *res = end_().node_;
                    //Идем циклом пока не дойдем до нуллптр(в пустом дереве, мы даже не зайдем в цикл)
                    while (begin != nullptr) {
                        if(!cmprt(begin->key_,key)) {
                            //если нашли элемент, то запоминаем его как предварительный,
                            //если найдем новые элементы(ниже по дереву), то обновим значение
                            res=begin;
                            begin=begin->left_;
                        } else {
                            //Если узел меньше, то идем вправо(там элементы больше текущего)
                            begin=begin->right_;
                        }
                    }
                    return iterator(res);
                }

                //аналогичная функция LowBow, только наоборот
                iterator UppBow(const_reference key) {
                    //начнем искать с корня
                    tree_node *begin = Root();
                    //Если ничего не найдем, то используется значение по-умолчанию(end)
                    tree_node *res = end_().node_;
                    //Идем циклом пока не дойдем до нуллптр(в пустом дереве, мы даже не зайдем в цикл)
                    while (begin != nullptr) {
                        if(cmprt(begin->key_,key)) {
                            //если нашли элемент, то запоминаем его как предварительный,
                            //если найдем новые элементы(ниже по дереву), то обновим значение
                            res=begin;
                            begin=begin->left_;
                        } else {
                            //Если узел меньше, то идем вправо(там элементы больше текущего)
                            begin=begin->right_;
                        }
                    }
                    return iterator(res);
                }

                //Удаление элемента на определенной позиции
                void Erase(iterator ind) noexcept {
                    tree_node *res = ExtractionNode(ind);
                    delete res;
                }

                //Проверка на корректность дерева
                bool TreeCheck() const noexcept {
                    //head дерева должна быть красной
                    if(head_->color_ == tBlack)
                        return false;
                    //пустое дерево-это всегда хорошо
                    if(Root() == nullptr)
                        return true;

                    //корень дерева всегда черный
                    if(Root()->color_ != tBlack)
                        return false;

                    //У красного узла все потомки черные
                    if(!RedCheckNode(Root()->color_))
                        return false;

                    //Любой простой путь от узла-предка до потомка содержит одинаковое кол-во черных узлов

                    if(BlackHeight(Root()) == -1)
                        return false;


                    //Если мы дошли до этого момента-поздравляю, дерево корректно
                    return true;
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
                            if(!uniq) tmp = tmp->right_;
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
                        SwapAndRemoveNode(removing_node,tmp);
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
                        SwapAndRemoveNode(removing_node,tmp);
                    }
                    //Обработки К0 и Ч0(так как с К0 нам никаких дополнительных обработок)
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
                        //пишем отдельную функцию
                        EraseB0(removing_node);
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

                //Функция извлечения и замены ноды(перестановка местами и удаление)
                void SwapAndRemoveNode(tree_node *removing_node, tree_node *tmp) noexcept{
                    //находим и меня ссылку на родителя у tmp на removing_node
                    if(tmp->parent_->left_==tmp)
                        tmp->parent_->left_=removing_node;
                    else
                        tmp->parent_->right_=removing_node;

                    //если removing_node корень-меняем на tmp
                    if(removing_node == Root())
                        Root()=tmp;
                    else {
                        //а если не корень, то меняем ссылку на узел removing_node у его родителя
                        if(removing_node->parent_->left_ ==removing_node)
                            removing_node->parent_->left_ = tmp;
                        else
                            removing_node->parent_->right_ = tmp;
                    }

                    //свапаем все кроме key_, ключи остаются на своем месте
                    std::swap(removing_node->parent_,tmp->parent_);
                    std::swap(removing_node->left_,tmp->left_);
                    std::swap(removing_node->right_,tmp->right_);
                    std::swap(removing_node->color_,tmp->color_);

                    //замена родителей у свапаемых нод
                    if(removing_node->right_)
                        removing_node->right_->parent_=removing_node;
                    if(removing_node -> left_)
                        removing_node->left_->parent_ = removing_node;
                    if(tmp->right_)
                        tmp->right_->parent_ = tmp;
                    if(tmp->left_)
                        tmp->left_->parent_ = tmp;
                }

                //отдельная функция для случая когда у нас в функции подается черная нода у которой нет детей
                //все правила отлично описаны на https://ru.wikipedia.org/wiki/Красно-чёрное_дерево в подразделе Удаление
                //Делалось все по алгоритм с этого сайта https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/
                void EraseB0 (tree_node* removing_node) noexcept {
                    tree_node *checked_node = removing_node;
                    tree_node *parent = removing_node->parent_;

                    //Делаем проверку в цикле
                    while(checked_node != Root() && checked_node->color_ == tBlack) {
                        if(checked_node ==parent->left_) {
                            //Значит узел который мы удаляем-слева от родителя
                            tree_node  *tmp = parent->right_;

                            //Случай первый
                            if(tmp->color_==tRed) {
                                std::swap(parent->color_,tmp-color_);
                                LeftRotate(parent);
                                parent = checked_node->parent_;
                                tmp = parent->right_;
                            }

                            //Случай второй
                            if(tmp->color_ == tBlack &&
                            (tmp->left_ == nullptr || tmp->left_->color_ == tBlack) &&
                            (tmp->right_ == nullptr || tmp->right_->color_ == tBlack)) {
                                tmp->color_ = tRed;
                                if(parent->color_ == tRed) {
                                    parent->color_ = tBlack;
                                    break;
                                }
                                //закончили с балансировкой, но нужно теперь заниматься балансировкой родителя
                                tmp = parent;
                                parent = tmp->parent_;
                            } else {
                                //тут уже будут третий и четвертый случаи
                                if(tmp->left_ != nullptr && tmp->left_->color_ ==tRed &&
                                 (tmp->right_ == nullptr || tmp->right_->color_ == tBlack)) {
                                    //собственно третий случай
                                    std::swap(tmp->color_,tmp->left_->color_);
                                    RightRotate(tmp);
                                    tmp = parent->right_;
                                }
                                //ну и последний случай
                                tmp->left_->color_=tBlack;
                                tmp->color_=parent->color_;
                                parent->color_=tBlack;
                                RightRotate(parent);
                                //Закончили с балансировкой
                                break;
                            }
                        } else {
                            //ну и осталось рассмотреть случай когда у нас не слева от родителя, а справа
                            tree_node *tmp =parent->right_;

                            //Первый случай
                            if(tmp->color_ == tRed) {
                                std::swap(tmp->color_,parent->color_);
                                RightRotate(parent);
                                parent = checked_node->parent_;
                                tmp = parent->left_;
                            }
                            //Второй случай
                            if(tmp->color_ == tBlack &&
                               (tmp->left_ == nullptr || tmp->left_->color_ == tBlack) &&
                               (tmp->right_ == nullptr || tmp->right_->color_ == tBlack)) {
                                tmp->color_ = tRed;
                                if (parent->color_ == tRed) {
                                    parent->color_ = tBlack;
                                    break;
                                }
                                //закончили с балансировкой, но нужно теперь заниматься балансировкой родителя
                                tmp = parent;
                                parent = tmp->parent_;
                            } else {
                                //тут уже будут третий и четвертый случаи
                                if(tmp->right_ != nullptr && tmp->right_->color_ ==tRed &&
                                   (tmp->left_ == nullptr || tmp->left_->color_ == tBlack)) {
                                    //собственно третий случай
                                    std::swap(tmp->color_,tmp->right_->color_);
                                    LeftRotate(tmp);
                                    tmp=tmp->left_;
                                }
                                //ну и последний случай
                                tmp->left_->color_=tBlack;
                                tmp->color_=parent->color_;
                                parent->color_=tBlack;
                                RightRotate(parent);
                                //Закончили с балансировкой
                                break;
                            }
                        }
                    }
                }

                //Рекурсивная функция. Основные принципы:
                // 1)В КЧ дереве не может быть двух подряд идущих красных узлов
                // 2)В КЧ дереве у красного родителя-дети черные
                bool RedCheckNode(const tree_node *knot)const noexcept {
                    if(knot->color_ ==tRed) {
                        if((knot->left_ != nullptr && knot->left_->color_ == tRed)||
                        (knot->right_ != nullptr && knot->right_->color_ == tRed))
                            return false;
                    }

                    //Если у нашего узла есть левый потомок
                    if(knot->left_ != nullptr) {
                        if (!RedCheckNode(knot->left_))
                            return false;
                    }

                    //Если у нашего узла есть правый потомок
                    if(knot->right_ != nullptr) {
                        if(!RedCheckNode(knot->right_))
                            return false;
                    }
                    //В остальных случаях возвращаем тру
                    return true;
                }

                //Расчитываем высоту дерева(помним про правило про просой путь)
                //Если дерево сбалансировано-возвращаем высоту, если нет возвращает -1
                int BlackHeight(const tree_node *knok) const noexcept {
                    //Пустое дерево всегда сбалансировано и имеет высоту 0
                    if(knok == nullptr)
                        return 0;

                    //Смотрим высоту (рекурсивно) для правых и левых потомков
                    int left_height = BlackHeight(knok->left_);
                    int right_height = BlackHeight(knok->right_);
                    //Если дерево некорректно-возвращает -1(может быть некорректными
                    // 1) Левое поддерево
                    // 2) Правое поддерево
                    // 3) Разная высота
                    // в остальных случаях, возвращаем высоту)
                    if(left_height != -1 && right_height != -1 && left_height == right_height ) {
                        int add = knok->color_ ==tBlack ? 1:0;
                        return add + left_height;
                        //так как left_height == right_height-неважно какую высоту мы прибавляем
                    } else
                        return -1;
                }


                reference operator *() const noexcept{
                    return node_->key_;
                }
            };
}

#endif //S21_CONTAINERS_SRC_S21_CONTAINERS_S21_TREE_H