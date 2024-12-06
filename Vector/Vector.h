#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include <memory>
#include <initializer_list>
#include <limits>
#include <algorithm>
#include <stdexcept>

template<typename T, typename Allocator = std::allocator<T>>
class Vector {
    using Type = T;

    struct Iterator {
        friend Vector;

        explicit Iterator(T* p);

        T& operator*() noexcept;
        Iterator& operator++() noexcept;
        Iterator& operator--() noexcept;
        bool operator!=(const Iterator& other) const noexcept;

    private:
        Type* ptr;
    };

    size_t m_size;
    size_t m_capacity;
    T* data;
    Allocator alloc;

public:
    Vector();
    explicit Vector(size_t& in_size);
    Vector(size_t& in_size, T& in_val);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;

    ~Vector();

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;
    T& operator[](const size_t& index) noexcept;
    const T& operator[](const size_t& index) const noexcept;

    T& at(size_t n);
    const T& at(size_t n) const;

    [[nodiscard]] size_t max_size() const noexcept;
    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] size_t capacity() const noexcept;

    [[nodiscard]] bool empty() const noexcept;
    bool reserve(const size_t& in_capacity);

    void shrink_to_fit() noexcept;
    void push_back(const T& in_val);
    void pop_back() noexcept;

    template<typename... Args>
    void emplace(size_t index, Args&&... args);

    template<typename... Args>
    void emplace_back(Args&&... args);

    void clear() noexcept;

    T& back();
    const T& back() const;
    T& front();
    const T& front() const;

    Iterator& begin();
    const Iterator& cbegin();

    Iterator& end();
    const Iterator& cend();
};

//Vector

template<typename T, typename Allocator>
Vector<T, Allocator>::Vector() : data(nullptr), m_size(0), m_capacity(0){}

template<typename T, typename Allocator>
Vector<T, Allocator>::Vector(size_t& in_size) {
    alloc.allocate(in_size);
    m_capacity = in_size;
    m_size = in_size;

    for (auto i = 0; i < m_size; i++)
        alloc.construct(data + i);
}

template<typename T, typename Allocator>
Vector<T, Allocator>::Vector(size_t &in_size, T &in_val) {
    alloc.allocate(in_size);
    m_capacity = in_size;
    m_size = in_size;

    for (auto i = 0; i < m_size; i++)
        alloc.construct(data + i, in_val);
}

template<typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Vector& other) : m_size(other.m_size), m_capacity(other.m_capacity){
    data = alloc.allocate(m_capacity);

    for (auto i = 0; i < m_size; i++)
        alloc.construct(data + i, other.data[i]);
}

template<typename T, typename Allocator>
Vector<T, Allocator>::Vector(Vector&& other) noexcept : data(other.date),
                        m_size(other.m_size), m_capacity(other.capacity()) {
    other.m_size = 0;
    other.m_capacity = 0;
    other.data = nullptr;
}

template<typename T, typename Allocator>
Vector<T, Allocator>::~Vector() {
    for (auto i = 0; i < m_size; i++)
        alloc.destroy(data + i);

    for (auto i = 0; i < m_capacity; i++)
        alloc.deallocate(data + i);
}

template<typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector& other) {
    if (this != &other) {
        for (auto i = 0; i < m_size; i++)
            alloc.destroy(data + i);

        for (auto i = 0; i < m_capacity; i++)
            alloc.deallocate(data + i);

        m_capacity = other.m_capacity;
        m_size = other.m_size;
        data = other.data;
    }
    return *this;
}

template<typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(Vector&& other) noexcept {
    if (this != &other) {
        for (auto i = 0; i < m_size; i++)
            alloc.destroy(data + i);

        for (auto i = 0; i < m_capacity; i++)
            alloc.deallocate(data + i);

        m_capacity = other.m_capacity;
        m_size = other.m_size;
        data = other.data;
        other.m_capacity = 0;
        other.m_size = 0;
        other.data = nullptr;
    }
    return *this;
}

template<typename T, typename Allocator>
T& Vector<T, Allocator>::operator[](const size_t& index) noexcept {
    return *(data + index);
}

template<typename T, typename Allocator>
const T& Vector<T, Allocator>::operator[](const size_t& index) const noexcept {
    return *(data + index);
}

template<typename T, typename Allocator>
T& Vector<T, Allocator>::at(size_t index) {
    if (index >= m_size)
        throw std::out_of_range("Index out of range");

    return data[index];
}

template<typename T, typename Allocator>
const T& Vector<T, Allocator>::at(size_t index) const {
    if (index >= m_size)
        throw std::out_of_range("Index out of range");

    return data[index];
}

template<typename T, typename Allocator>
[[nodiscard]] size_t Vector<T, Allocator>::max_size() const noexcept {
    return std::numeric_limits<T>::max() / sizeof(T);
}

template<typename T, typename Allocator>
[[nodiscard]] size_t Vector<T, Allocator>::capacity() const noexcept {
    return m_capacity;
}

template<typename T, typename Allocator>
[[nodiscard]] size_t Vector<T, Allocator>::size() const noexcept {
    return m_size;
}

template<typename T, typename Allocator>
[[nodiscard]] bool Vector<T, Allocator>::empty() const noexcept {
    return !m_size;
}

template<typename T, typename Allocator>
bool Vector<T, Allocator>::reserve(const size_t& in_capacity) {
    if (in_capacity <= m_capacity)
        return false;

    T* newData = alloc.allocate(in_capacity);

    for (auto i = 0; i < m_size; i++) {
        alloc.construct(newData + i, std::move(data[i]));
        alloc.destroy(data + i);
    }

    alloc.deallocate(data, m_capacity);
    data = newData;
    m_capacity = in_capacity;
    return true;
}

template<typename T, typename Allocator>
void Vector<T, Allocator>::shrink_to_fit() noexcept {
    alloc.deallocate(data + m_size, m_capacity - m_size);
    m_capacity = m_size;
}

template<typename T, typename Allocator>
void Vector<T, Allocator>::push_back(const T& in_val) {
    if (m_size == m_capacity) {
        m_capacity = (1 + m_capacity) * 2;
        T* newData = alloc.allocate(m_capacity);

        for (auto i = 0; i < m_size; i++) {
            alloc.construct(newData + i, std::move(data[i]));
            alloc.destroy(data + i);
        }

        alloc.deallocate(data, m_size);
        data = newData;
    }
    alloc.construct(data + m_size, in_val);
    m_size++;
}

template<typename T, typename Allocator>
void Vector<T, Allocator>::pop_back() noexcept {
    if (m_size == 0)
        return;

    m_size--;
    alloc.destroy(data + m_size);
}

template<typename T, typename Allocator>
template<typename... Args>
void Vector<T, Allocator>::emplace(size_t index, Args&&... args) {
    if (index > m_size)
        throw std::out_of_range("Index out of range");

    if (m_size >= m_capacity)
        reserve(m_capacity == 0 ? 2 : m_capacity * 2);

    for (size_t i = m_size; i > index; --i) {
        alloc.construct(&data[i], std::move(data[i - 1]));
        alloc.destroy(&data[i - 1]);
    }
    alloc.construct(&data[index], std::forward<Args>(args)...);
    m_size++;
}

template<typename T, typename Allocator>
template<typename... Args>
void Vector<T, Allocator>::emplace_back(Args&&... args) {
    if (m_size >= m_capacity)
        reserve(m_capacity == 0 ? 2 : m_capacity * 2);

    T* ptr = alloc.allocate(1);
    alloc.construct(ptr, std::forward<Args>(args)...);
    data[m_size] = *ptr;
    m_size++;
}


template<typename T, typename Allocator>
void Vector<T, Allocator>::clear() noexcept {
    for (auto i = 0; i < m_size; i++)
        alloc.destroy(data + i);

    m_size = 0;
}

template<typename T, typename Allocator>
T& Vector<T, Allocator>::back() {
    if (m_size == 0)
        throw std::out_of_range("Vector is empty");

    return data[m_size - 1];
}

template<typename T, typename Allocator>
const T& Vector<T, Allocator>::back() const {
    if (m_size == 0)
        throw std::out_of_range("Vector is empty");

    return data[m_size - 1];
}

template<typename T, typename Allocator>
T& Vector<T, Allocator>::front() {
    if (m_size == 0)
        throw std::out_of_range("Vector is empty");

    return data[0];
}

template<typename T, typename Allocator>
const T& Vector<T, Allocator>::front() const {
    if (m_size == 0)
        throw std::out_of_range("Vector is empty");

    return data[0];
}

//Iterator

template<typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator& Vector<T, Allocator>::begin() {
    return Iterator(data);
}

template<typename T, typename Allocator>
const typename Vector<T, Allocator>::Iterator& Vector<T, Allocator>::cbegin() {
    return Iterator(data);
}

template<typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator& Vector<T, Allocator>::end(){
    return Iterator(data + m_size);
}

template<typename T, typename Allocator>
const typename Vector<T, Allocator>::Iterator& Vector<T, Allocator>::cend() {
    return Iterator(data + m_size);
}


template<typename T, typename Allocator>
Vector<T, Allocator>::Iterator::Iterator(T* p) : ptr(p) {}

template<typename T, typename Allocator>
T& Vector<T, Allocator>::Iterator::operator*() noexcept {
    return *ptr;
}

template<typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator& Vector<T, Allocator>::Iterator::operator++() noexcept {
    ptr++;
    return *this;
}

template<typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator& Vector<T, Allocator>::Iterator::operator--() noexcept {
    ptr--;
    return *this;
}

template<typename T, typename Allocator>
bool Vector<T, Allocator>::Iterator::operator!=(const Iterator& other) const noexcept {
    return ptr != other.ptr;
}

#endif //VECTOR_VECTOR_H
