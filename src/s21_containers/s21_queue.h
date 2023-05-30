#include <cstddef>
#include "s21_list.h"

template <typename T>
class Queue {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;

    Queue() = default;
    Queue(const Queue& other);
    Queue(Queue&& other) noexcept;
    Queue& operator=(const Queue& other);
    Queue& operator=(Queue&& other) noexcept;
    ~Queue();

    const_reference front() const;
    const_reference back() const;
    bool empty() const;
    size_type size() const;
    void push(const_reference value);
    void pop();
    void swap(Queue& other);
    template <class... Args>
    void emplace_back(Args&&... args);

private:
    List<value_type> list_;
};

template <typename T>
Queue<T>::Queue(const Queue& other) : list_{ other.list_ } {}

template <typename T>
Queue<T>::Queue(Queue&& other) noexcept : list_{ std::move(other.list_) } {}

template <typename T>
Queue<T>& Queue<T>::operator=(const Queue& other) {
    if (this != &other) {
        Queue(other).swap(*this);
    }
    return *this;
}

template <typename T>
Queue<T>& Queue<T>::operator=(Queue&& other) noexcept {
    if (this != &other) {
        list_ = std::move(other.list_);
    }
    return *this;
}

template <typename T>
Queue<T>::~Queue() {}

template <typename T>
typename Queue<T>::const_reference Queue<T>::front() const {
    return list_.front();
}

template <typename T>
typename Queue<T>::const_reference Queue<T>::back() const {
    return list_.back();
}

template <typename T>
bool Queue<T>::empty() const {
    return list_.empty();
}

template <typename T>
typename Queue<T>::size_type Queue<T>::size() const {
    return list_.size();
}

template <typename T>
void Queue<T>::push(const_reference value) {
    list_.push_back(value);
}

template <typename T>
void Queue<T>::pop() {
    list_.pop_front();
}

template <typename T>
void Queue<T>::swap(Queue& other) {
    list_.swap(other.list_);
}

template <typename T>
template <class... Args>
void Queue<T>::emplace_back(Args&&... args) {
    list_.emplace_back(std::forward<Args>(args)...);
}