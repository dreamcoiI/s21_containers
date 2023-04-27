#ifndef S21_CONTAINERS_S21_CONTAINERS_S21_VECTOR_H_
#define S21_CONTAINERS_S21_CONTAINERS_S21_VECTOR_H_
#include <algorithm>
#include <initializer_list>
#include <limits>
#include <utility>

namespace s21 {
    template<typename T>
    class vector {
    public:
        using value_type = T;
        using reference = T &;
        using const_reference = const T &;
        using iterator = T *;
        using const_iterator = const T *;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
    public:
        vector() {
        }

        explicit vector(size_type size) {
            size_ = size;
            capacity_ = size;
            buffer_ = nullptr;
            if (size > 0) {
                buffer_ = new value_type[capacity_];
            }
        }

        explicit vector(std::initializer_list<value_type> const &init)
                : size_{init.size()},
                  capacity_(init.size()), buffer_{new value_type[capacity_]} {
            std::copy(init.begin(), init.end(), buffer_);
        }

        vector(const vector &cpy) {
            size_ = cpy.size_;
            capacity_ = cpy.capacity_;
            buffer_ = nullptr;
            if (size > 0) {
                buffer_ = new value_type[capacity_];
            }
            std::copy(cpy.begin(), cpy.end(), buffer_);
        }

        vector(vector &&mcv) noexcept {
            size_ = std::exchange(mcv.size_, 0);
            capacity_ = std::exchange(mcv.capacity_, 0);
            buffer_ = std::exchange(mcv.buffer_, 0);
        }

        ~vector() {
            delete buffer_;
        }

        vector &operator=(vector &mcv) noexcept {
            if (this != &mcv) {
                size_ = std::exchange(mcv.size_, 0);
                capacity_ = std::exchange(mcv.capacity_, 0);
                buffer_ = std::exchange(mcv.buffer_, 0);
            }
            return *this;
        }

        vector &operator=(const vector &mcv) {
            if(this != &mcv) {
                delete[] buffer_;
                if(mcv.size_ > 0) {
                    buffer_ = new value_type[mcv.capacity_];
                    std::copy(mcv.begin(),mcv.end(),buffer_);

                }
                size_ = mcv.size;
                capacity_=mcv.capacity;
            }
            return  *this;
        }
    public:
        reference at(size_type ind) {
            if( ind >= size_) {
                throw std::logic_error(
                        'vector::The index is out of range');

            }
            return  buffer[ind];
        }
        const_reference at(size_type ind) const {
            if(ind > size_) {
                throw std::logic_error(
                        'vector::The index is out of range');

            }
            return  buffer[ind];
        }

        constexpr reference operator [](size_type ind) {
            return at[ind];
        }
        constexpr const_reference operator[](size_type ind) const {
            return  at[ind];
        }
        


    private:
        size_type size_= 0;
        size_type capacity_ = 0;
        iterator buffer_ = nullptr;

        void RemoveVector(size_type capacity) {
            auto temp = new value_type[capacity];
            for(auto i = 0; i < size_;++i)
                temp[i] = std::move(buffer_[i]);
            delete[] buffer_;
            buffer_ = temp;
            capacity_ = capacity;
        }
    };
} //namespace
#endif  // S21_CONTAINERS_S21_CONTAINERS_S21_VECTOR_H_