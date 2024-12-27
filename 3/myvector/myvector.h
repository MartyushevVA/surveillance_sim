#pragma once 

#include <cstddef>
#include <type_traits>
#include <concepts>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <limits>

/**
 * @class Vector
 * @brief A dynamic array class template that provides similar functionality to std::vector.
 * 
 * @tparam T The type of elements stored in the vector.
 */
template<typename T>
class Vector {
    size_t size_; ///< Current number of elements in the vector.
    size_t capacity_; ///< Current capacity of the vector.
    T* data_; ///< Pointer to the dynamically allocated array of elements.

    /**
     * @brief Reallocates the vector to a new capacity.
     * 
     * @param new_capacity The new capacity for the vector.
     */
    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size_; ++i) new_data[i] = std::move(data_[i]);
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

public:
    /**
     * @class VectorIterator
     * @brief An iterator class for the Vector, supporting random access.
     * 
     * @tparam is_const Boolean indicating if the iterator is const.
     */
    template<bool is_const>
    class VectorIterator {
    public:
        using node_ptr_t = std::conditional_t<is_const, const T, T>*;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = std::conditional_t<is_const, const T*, T*>;
        using reference = std::conditional_t<is_const, const T&, T&>;

        /**
         * @brief Default constructor for VectorIterator.
         */
        VectorIterator() noexcept : node(nullptr) {}

        /**
         * @brief Constructs a VectorIterator pointing to a specific node.
         * 
         * @param node Pointer to the node.
         */
        VectorIterator(node_ptr_t node) : node(node) {}

        /**
         * @brief Copy constructor for VectorIterator.
         * 
         * @tparam other_const Boolean indicating if the other iterator is const.
         * @param o The other iterator to copy from.
         */
        template<bool other_const>
        VectorIterator(const VectorIterator<other_const>& o) noexcept {
            node = o.node;
        }

        /**
         * @brief Copy assignment operator for VectorIterator.
         * 
         * @tparam other_const Boolean indicating if the other iterator is const.
         * @param o The other iterator to assign from.
         * @return VectorIterator& Reference to the assigned iterator.
         */
        template<bool other_const>
        VectorIterator& operator=(const VectorIterator<other_const>& o) noexcept {
            node = o.node;
            return *this;
        }

        /**
         * @brief Dereference operator.
         * 
         * @return reference Reference to the pointed-to element.
         */
        reference operator*() const noexcept { return *node; }

        /**
         * @brief Arrow operator.
         * 
         * @return pointer Pointer to the pointed-to element.
         */
        pointer operator->() const noexcept { return node; }

        /**
         * @brief Subscript operator.
         * 
         * @param n Index offset from the current position.
         * @return reference Reference to the element at the offset.
         */
        reference operator[](difference_type n) const noexcept { return node[n]; }

        /**
         * @brief Pre-increment operator.
         * 
         * @return VectorIterator& Reference to the incremented iterator.
         */
        VectorIterator& operator++() noexcept { ++node; return *this; }

        /**
         * @brief Post-increment operator.
         * 
         * @return VectorIterator Copy of the iterator before increment.
         */
        VectorIterator operator++(int) noexcept { VectorIterator temp = *this; ++node; return temp; }

        /**
         * @brief Pre-decrement operator.
         * 
         * @return VectorIterator& Reference to the decremented iterator.
         */
        VectorIterator& operator--() noexcept { --node; return *this; }

        /**
         * @brief Post-decrement operator.
         * 
         * @return VectorIterator Copy of the iterator before decrement.
         */
        VectorIterator operator--(int) noexcept { VectorIterator temp = *this; --node; return temp; }

        /**
         * @brief Addition assignment operator.
         * 
         * @param n Offset to add.
         * @return VectorIterator& Reference to the updated iterator.
         */
        VectorIterator& operator+=(difference_type n) noexcept { node += n; return *this; }

        /**
         * @brief Subtraction assignment operator.
         * 
         * @param n Offset to subtract.
         * @return VectorIterator& Reference to the updated iterator.
         */
        VectorIterator& operator-=(difference_type n) noexcept { node -= n; return *this; }

        /**
         * @brief Addition operator.
         * 
         * @param n Offset to add.
         * @return VectorIterator New iterator at the offset position.
         */
        VectorIterator operator+(difference_type n) const noexcept { return VectorIterator(node + n); }

        /**
         * @brief Subtraction operator.
         * 
         * @param n Offset to subtract.
         * @return VectorIterator New iterator at the offset position.
         */
        VectorIterator operator-(difference_type n) const noexcept { return VectorIterator(node - n); }

        /**
         * @brief Subtraction operator for iterators.
         * 
         * @param other The other iterator to subtract.
         * @return difference_type The difference between the iterators.
         */
        difference_type operator-(const VectorIterator& other) const noexcept { return node - other.node; }

        /**
         * @brief Three-way comparison operator.
         * 
         * @tparam other_const Boolean indicating if the other iterator is const.
         * @param other The other iterator to compare with.
         * @return std::strong_ordering The result of the comparison.
         */
        template<bool other_const>
        auto operator<=>(const VectorIterator<other_const>& other) const noexcept {
            return node <=> other.node;
        }

        /**
         * @brief Equality operator.
         * 
         * @param other The other iterator to compare with.
         * @return bool True if the iterators are equal, false otherwise.
         */
        template<bool other_const>
        bool operator==(const VectorIterator<other_const>& other) const {return node == other.node;}

        /**
         * @brief Inequality operator.
         * 
         * @param other The other iterator to compare with.
         * @return bool True if the iterators are not equal, false otherwise.
         */
        template<bool other_const>
        bool operator!=(const VectorIterator<other_const>& other) const {return node != other.node;}

        /**
         * @brief Friend addition operator for offset and iterator.
         * 
         * @param n Offset to add.
         * @param it The iterator to add the offset to.
         * @return VectorIterator New iterator at the offset position.
         */
        friend VectorIterator operator+(difference_type n, const VectorIterator& it) noexcept {
            return it + n;
        }

    private:
        node_ptr_t node; ///< Pointer to the current element in the vector.

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

    /**
     * @brief Default constructor for Vector.
     */
    Vector() noexcept : size_(0), capacity_(0), data_(nullptr) {}

    /**
     * @brief Constructs a Vector with a specified initial capacity.
     * 
     * @param capacity Initial capacity of the vector.
     */
    explicit Vector(size_t capacity) : size_(0), capacity_(capacity), data_(new T[capacity]) {}

    /**
     * @brief Constructs a Vector from an initializer list.
     * 
     * @param init_list Initializer list to initialize the vector.
     */
    Vector(std::initializer_list<T> init_list) : size_(init_list.size()), capacity_(init_list.size()), data_(new T[init_list.size()]) {
        size_t i = 0;
        for (const T& val : init_list) data_[i++] = val;
    }

    /**
     * @brief Copy constructor for Vector.
     * 
     * @param other The vector to copy from.
     */
    Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_), data_(new T[other.capacity_]) {
        for (size_t i = 0; i < size_; ++i) data_[i] = other.data_[i];
    }

    /**
     * @brief Move constructor for Vector.
     * 
     * @param other The vector to move from.
     */
    Vector(Vector&& other) noexcept : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }

    /**
     * @brief Destructor for Vector.
     */
    ~Vector() { delete[] data_; }

    /**
     * @brief Copy assignment operator for Vector.
     * 
     * @param other The vector to copy from.
     * @return Vector& Reference to the assigned vector.
     */
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

    /**
     * @brief Move assignment operator for Vector.
     * 
     * @param other The vector to move from.
     * @return Vector& Reference to the assigned vector.
     */
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

    /**
     * @brief Adds an element to the end of the vector.
     * 
     * @param value The value to add.
     */
    void push_back(const T& value) noexcept {
        if (size_ == capacity_) reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        data_[size_] = value;
        ++size_;
    }

    /**
     * @brief Adds an element to the end of the vector using move semantics.
     * 
     * @param value The value to add.
     */
    void push_back(T&& value) {
        if (size_ == capacity_) reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        data_[size_] = std::move(value);
        ++size_;
    }

    /**
     * @brief Removes the last element of the vector.
     * 
     * @throws std::out_of_range if the vector is empty.
     */
    void pop_back() {
        if (size_ == 0) throw std::out_of_range("Vector is empty");
        --size_;
    }

    /**
     * @brief Accesses an element by index.
     * 
     * @param index The index of the element.
     * @return T& Reference to the element.
     */
    T& operator[](size_t index) { return data_[index]; }

    /**
     * @brief Accesses an element by index (const version).
     * 
     * @param index The index of the element.
     * @return const T& Const reference to the element.
     */
    const T& operator[](size_t index) const { return data_[index]; }

    /**
     * @brief Gets the current size of the vector.
     * 
     * @return size_t The number of elements in the vector.
     */
    size_t size() const noexcept { return size_; }

    /**
     * @brief Gets the maximum possible size of the vector.
     * 
     * @return size_t The maximum number of elements the vector can hold.
     */
    size_t max_size() const noexcept { return std::numeric_limits<size_t>::max(); }

    /**
     * @brief Gets the current capacity of the vector.
     * 
     * @return size_t The capacity of the vector.
     */
    size_t capacity() const noexcept { return capacity_; }

    /**
     * @brief Checks if the vector is empty.
     * 
     * @return bool True if the vector is empty, false otherwise.
     */
    bool empty() const noexcept { return size_ == 0; }

    /**
     * @brief Clears all elements from the vector.
     */
    void clear() noexcept { size_ = 0; }

    /**
     * @brief Swaps the contents of this vector with another.
     * 
     * @param other The vector to swap with.
     */
    void swap(Vector& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
    }

    /**
     * @brief Reserves capacity for the vector.
     * 
     * @param new_capacity The new capacity to reserve.
     */
    void reserve(size_t new_capacity) noexcept { if (new_capacity > capacity_) reallocate(new_capacity); }

    /**
     * @brief Resizes the vector to a new size.
     * 
     * @param new_size The new size of the vector.
     */
    void resize(size_t new_size) noexcept { if (new_size > size_) resize(new_size, T()); }

    /**
     * @brief Resizes the vector to a new size, filling new elements with a specified value.
     * 
     * @param new_size The new size of the vector.
     * @param value The value to fill new elements with.
     */
    void resize(size_t new_size, const T& value) noexcept {
        if (new_size > size_) {
            reserve(new_size);
            for (size_t i = size_; i < new_size; ++i)
                data_[i] = value;
        }
        size_ = new_size;
    }

    /**
     * @brief Reduces the capacity of the vector to fit its size.
     */
    void shrink_to_fit() noexcept { if (size_ < capacity_) { reallocate(size_); } }

    /**
     * @brief Gets a reference to the first element.
     * 
     * @return T& Reference to the first element.
     */
    T& front() noexcept { return data_[0]; }

    /**
     * @brief Gets a const reference to the first element.
     * 
     * @return const T& Const reference to the first element.
     */
    const T& front() const noexcept { return data_[0]; }

    /**
     * @brief Gets a reference to the last element.
     * 
     * @return T& Reference to the last element.
     */
    T& back() noexcept { return data_[size_ - 1]; }

    /**
     * @brief Gets a const reference to the last element.
     * 
     * @return const T& Const reference to the last element.
     */
    const T& back() const noexcept { return data_[size_ - 1]; }

    /**
     * @brief Gets a pointer to the underlying data array.
     * 
     * @return T* Pointer to the data array.
     */
    T* data() noexcept { return data_; }

    /**
     * @brief Gets a const pointer to the underlying data array.
     * 
     * @return const T* Const pointer to the data array.
     */
    const T* data() const noexcept { return data_; }

    /**
     * @brief Returns an iterator to the beginning of the vector.
     * 
     * @return iterator Iterator to the first element.
     */
    iterator begin() noexcept { return iterator(data_); }

    /**
     * @brief Returns an iterator to the end of the vector.
     * 
     * @return iterator Iterator to the element following the last element.
     */
    iterator end() noexcept { return iterator(data_ + size_); }

    /**
     * @brief Returns a const iterator to the beginning of the vector.
     * 
     * @return const_iterator Const iterator to the first element.
     */
    const_iterator begin() const noexcept { return const_iterator(data_); }

    /**
     * @brief Returns a const iterator to the end of the vector.
     * 
     * @return const_iterator Const iterator to the element following the last element.
     */
    const_iterator end() const noexcept { return const_iterator(data_ + size_); }

    /**
     * @brief Returns a reverse iterator to the beginning of the reversed vector.
     * 
     * @return reverse_iterator Reverse iterator to the first element of the reversed vector.
     */
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

    /**
     * @brief Returns a const reverse iterator to the beginning of the reversed vector.
     * 
     * @return const_reverse_iterator Const reverse iterator to the first element of the reversed vector.
     */
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }

    /**
     * @brief Returns a reverse iterator to the end of the reversed vector.
     * 
     * @return reverse_iterator Reverse iterator to the element following the last element of the reversed vector.
     */
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

    /**
     * @brief Returns a const reverse iterator to the end of the reversed vector.
     * 
     * @return const_reverse_iterator Const reverse iterator to the element following the last element of the reversed vector.
     */
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

    /**
     * @brief Inserts an element at a specified position.
     * 
     * @param pos Iterator to the position to insert at.
     * @param value The value to insert.
     * @return iterator Iterator to the inserted element.
     */
    iterator insert(const_iterator pos, const T& value) noexcept
    requires std::is_copy_constructible_v<T> {
        if (pos < begin() || pos > end()) throw std::out_of_range("Iterator out of range");
        size_t index = pos - begin();
        if (size_ == capacity_) reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        std::move_backward(data_ + index, data_ + size_, data_ + size_ + 1);
        new (data_ + index) T(value);
        ++size_;
        return data_ + index;
    }

    /**
     * @brief Inserts an element at a specified position using move semantics.
     * 
     * @param pos Iterator to the position to insert at.
     * @param value The value to insert.
     * @return iterator Iterator to the inserted element.
     */
    iterator insert(const_iterator pos, T&& value) noexcept 
    requires std::is_move_constructible_v<T> {
        if (size_ == capacity_) reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        iterator it = iterator(data_ + (pos - begin()));
        std::move_backward(it, end(), end() + 1);
        new (data_ + (pos - begin())) T(std::move(value));
        ++size_;
        return it;
    }

    /**
     * @brief Erases an element at a specified position.
     * 
     * @param pos Iterator to the position to erase.
     * @return iterator Iterator to the element following the erased element.
     */
    iterator erase(const_iterator pos)
    requires std::is_move_constructible_v<T> {
        if (pos < begin() || pos >= end()) throw std::out_of_range("Iterator out of range");
        iterator it = iterator(data_ + (pos - begin()));
        std::move(it + 1, end(), it);
        --size_;
        return it;
    }

    /**
     * @brief Erases a range of elements.
     * 
     * @param first Iterator to the first element to erase.
     * @param last Iterator to the element following the last element to erase.
     * @return iterator Iterator to the element following the last erased element.
     */
    iterator erase(const_iterator first, const_iterator last)
    requires std::is_move_constructible_v<T> {
        if (first < begin() || last > end()) throw std::out_of_range("Iterators out of range");
        iterator it = iterator(data_ + (first - begin()));
        std::move(data_ + (last - begin()), data_ + size_, data_ + (first - begin()));
        size_ -= last - first;
        return it;
    }

    friend class VectorIterator<false>;
    friend class VectorIterator<true>;
};