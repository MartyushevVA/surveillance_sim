#pragma once 

#include <cstddef>
#include <type_traits>
#include <concepts>
#include <iterator>
#include <algorithm>

template<typename T>
class Vector {
    size_t size_;
    size_t capacity_;
    T* data_;

    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

public:

    template<bool is_const>
    class VectorIterator {
    public:
        using node_ptr_t = std::conditional_t<is_const, const T, T>*;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = std::conditional_t<is_const, const T*, T*>;
        using reference = std::conditional_t<is_const, const T&, T&>;

        VectorIterator() noexcept : node(nullptr) {}
        VectorIterator(node_ptr_t node) : node(node) {}

        template<bool other_const>
        VectorIterator(const VectorIterator<other_const>& o) noexcept
        requires (is_const >= other_const) : node(o.node) {}

        template<bool other_const>
        VectorIterator& operator=(const VectorIterator<other_const>& o) noexcept
        requires (is_const >= other_const) {
            node = o.node;
            return *this;
        }

        reference operator*() const noexcept { return *node; }
        pointer operator->() const noexcept { return node; }
        reference operator[](difference_type n) const noexcept { return node[n]; }

        VectorIterator& operator++() noexcept { ++node; return *this; }
        VectorIterator operator++(int) noexcept { VectorIterator temp = *this; ++node; return temp; }
        VectorIterator& operator--() noexcept { --node; return *this; }
        VectorIterator operator--(int) noexcept { VectorIterator temp = *this; --node; return temp; }

        VectorIterator& operator+=(difference_type n) noexcept { node += n; return *this; }
        VectorIterator& operator-=(difference_type n) noexcept { node -= n; return *this; }
        VectorIterator operator+(difference_type n) const noexcept { VectorIterator temp = *this; return temp += n; }
        difference_type operator+(const VectorIterator& other) const noexcept { return node + other.node; }
        VectorIterator operator-(difference_type n) const noexcept { VectorIterator temp = *this; return temp -= n; }
        difference_type operator-(const VectorIterator& other) const noexcept { return node - other.node; }

        template<bool other_const>
        auto operator<=>(const VectorIterator<other_const>& other) const noexcept {
            return node <=> other.node;
        }

        bool operator==(const VectorIterator& other) const noexcept { return node == other.node; }
        bool operator!=(const VectorIterator& other) const noexcept { return node != other.node; }
        bool operator<(const VectorIterator& other) const noexcept { return node < other.node; }
        bool operator<=(const VectorIterator& other) const noexcept { return node <= other.node; }
        bool operator>(const VectorIterator& other) const noexcept { return node > other.node; }
        bool operator>=(const VectorIterator& other) const noexcept { return node >= other.node; }

        friend VectorIterator operator+(difference_type n, const VectorIterator& it) noexcept {
            return it + n;
        }

    private:
        node_ptr_t node;

        friend Vector<T>;
        friend VectorIterator<!is_const>;
    };

    using size_type = size_t;

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    
    using iterator = VectorIterator<false>;
    using const_iterator = VectorIterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Vector() noexcept : size_(0), capacity_(0), data_(nullptr) {}
    explicit Vector(size_t capacity) : size_(0), capacity_(capacity), data_(new T[capacity]) {}
    Vector(std::initializer_list<T> init_list) : size_(init_list.size()), capacity_(init_list.size()), data_(new T[init_list.size()]) {
        size_t i = 0;
        for (const T& val : init_list) data_[i++] = val;
    }
    Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_), data_(new T[other.capacity_]) {
        for (size_t i = 0; i < size_; ++i) data_[i] = other.data_[i];
    }
    Vector(Vector&& other) noexcept : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }
    ~Vector() { delete[] data_; }

    Vector& operator=(const Vector& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) data_[i] = other.data_[i];
        }
        return *this;
    }
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = other.data_;
            other.size_ = 0;
            other.capacity_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    void push_back(const T& value) noexcept {
        if (size_ == capacity_) reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        data_[size_] = value;
        ++size_;
    }
    void push_back(T&& value) {
        if (size_ == capacity_) reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        data_[size_] = std::move(value);
        ++size_;
    }
    void pop_back() {
        if (size_ == 0) throw std::out_of_range("Vector is empty");
        --size_;
    };
    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }

    size_t size() const noexcept { return size_; }
    size_t max_size() const noexcept { return std::numeric_limits<size_t>::max(); }
    size_t capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return size_ == 0; }
    void clear() noexcept { size_ = 0; }
    void swap(Vector& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
    }

    void reserve(size_t new_capacity) noexcept {if (new_capacity > capacity_) reallocate(new_capacity);}
    void resize(size_t new_size) noexcept {if (new_size > size_) resize(new_size, T());}
    void resize(size_t new_size, const T& value) noexcept {
        if (new_size > size_) {
            reserve(new_size);
            for (size_t i = size_; i < new_size; ++i)
                data_[i] = value;
        }
        size_ = new_size;
    }
    void shrink_to_fit() noexcept {if (size_ < capacity_) {reallocate(size_);}}

    T& front() noexcept { return data_[0]; }
    const T& front() const noexcept { return data_[0]; }
    T& back() noexcept { return data_[size_ - 1]; }
    const T& back() const noexcept { return data_[size_ - 1]; }
    T* data() noexcept { return data_; }
    const T* data() const noexcept { return data_; }

    template<typename... Args>
    void emplace_back(Args&&... args) noexcept
    requires std::is_constructible_v<T, Args...> {
        if (size_ == capacity_) reallocate(capacity_ * 2);
        new (data_ + size_) T(std::forward<Args>(args)...);
        ++size_;
    };
    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) noexcept
    requires std::is_constructible_v<T, Args...> {
        if (size_ == capacity_) reallocate(capacity_ * 2);
        iterator it = iterator(data_ + (pos - begin()));
        new (data_ + (pos - begin())) T(std::forward<Args>(args)...);
        ++size_;
        return it;
    };
    iterator insert(const_iterator pos, const T& value) noexcept
    requires std::is_copy_constructible_v<T> {
        if (size_ == capacity_) reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        iterator it = iterator(data_ + (pos - begin()));
        std::move_backward(it, end(), end() + 1);
        new (data_ + (pos - begin())) T(value);
        ++size_;
        return it;
    };
    iterator insert(const_iterator pos, T&& value) noexcept 
    requires std::is_move_constructible_v<T> {
        if (size_ == capacity_) reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        iterator it = iterator(data_ + (pos - begin()));
        std::move_backward(it, end(), end() + 1);
        new (data_ + (pos - begin())) T(std::move(value));
        ++size_;
        return it;
    }
    iterator erase(const_iterator pos)
    requires std::is_move_constructible_v<T> {
        if (pos < begin() || pos >= end()) throw std::out_of_range("Iterator out of range");
        iterator it = iterator(data_ + (pos - begin()));
        std::move(it + 1, end(), it);
        --size_;
        return it;
    }
    iterator erase(const_iterator first, const_iterator last)
    requires std::is_move_constructible_v<T> {
        if (first < begin() || last > end()) throw std::out_of_range("Iterators out of range");
        iterator it = iterator(data_ + (first - begin()));
        std::move(data_ + (last - begin()), data_ + size_, data_ + (first - begin()));
        size_ -= last - first;
        return it;
    }

    iterator begin() noexcept { return iterator(data_); }
    iterator end() noexcept { return iterator(data_ + size_); }
    const_iterator begin() const noexcept { return const_iterator(data_); }
    const_iterator end() const noexcept { return const_iterator(data_ + size_); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
};