#ifndef LIST_LIST_H
#define LIST_LIST_H

#include <memory>
#include <stdexcept>
#include <utility>
#include <limits>

template<typename T, typename Allocator = std::allocator<T>>
class List{
    using Type = T;

    struct ListNode {
        friend List;

        ListNode();
        explicit ListNode(Type& in_val);
        ListNode(Type& in_val, ListNode* in_next, ListNode* in_prev);

    private:
        Type val;
        ListNode* next;
        ListNode* prev;
    };

    struct Iterator {
        friend List;

        explicit Iterator(ListNode* in_ptr);

        Type& operator*();
        Type* operator->();
        Iterator& operator++() noexcept;
        Iterator& operator--() noexcept;
        bool operator!=(const Iterator& other) const noexcept;

    private:
        ListNode* ptr;
    };

    ListNode* m_head;
    ListNode* m_tail;
    size_t m_size;
    Allocator alloc;

    void deleter() noexcept;

public:
    List();
    List(const List& other);
    List(List&& other) noexcept;

    ~List() noexcept;

    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] bool find(const T& in_val) const noexcept;

    Type& back() const;
    Type& front() const;

    void reverse() noexcept;
    void swap(List& other) noexcept;

    Allocator& getAllocator() const noexcept;

    void pop(const size_t& pos);
    void pop_back() noexcept;
    void pop_front() noexcept;

    void insert(const size_t& pos, const Type& in_val);
    void push_back(const Type& in_val);
    void push_front(const Type& in_val);

    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] size_t max_size() const noexcept;

    Iterator& begin();
    const Iterator& cbegin() const;

    Iterator& end();
    const Iterator& cend() const;
};

//ListNode

template<typename T, typename Allocator>
List<T, Allocator>::ListNode::ListNode() : val(), next(nullptr), prev(nullptr) {}

template<typename T, typename Allocator>
List<T, Allocator>::ListNode::ListNode(T& in_val) : val(in_val), next(nullptr), prev(nullptr) {}

template<typename T, typename Allocator>
List<T, Allocator>::ListNode::ListNode(T& in_val, ListNode* in_next, ListNode* in_prev) :
                                val(in_val), next(in_next), prev(in_prev) {}

//Iterator

template<typename T, typename Allocator>
List<T, Allocator>::Iterator::Iterator(ListNode* in_ptr) : ptr(in_ptr) {}

template<typename T, typename Allocator>
T& List<T, Allocator>::Iterator::operator*() {
    if (!ptr) {
        throw std::out_of_range("Iterator is out of range");
    }
    return ptr->val;
}

template<typename T, typename Allocator>
T* List<T, Allocator>::Iterator::operator->() {
    if (!ptr) {
        throw std::out_of_range("Iterator is out of range");
    }
    return &(ptr->val);
}

template<typename T, typename Allocator>
typename List<T,Allocator>::Iterator& List<T, Allocator>::Iterator::operator++() noexcept {
    if (ptr)
        ptr = ptr->next;
    return *this;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::Iterator& List<T, Allocator>::Iterator::operator--() noexcept {
    if (ptr)
        ptr = ptr->prev;
    return *this;
}

template<typename T, typename Allocator>
bool List<T, Allocator>::Iterator::operator!=(const List::Iterator &other) const noexcept {
    return ptr != other.ptr;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::Iterator& List<T, Allocator>::begin() {
    return Iterator(m_head);
}

template<typename T, typename Allocator>
const typename List<T,Allocator>::Iterator& List<T, Allocator>::cbegin() const {
    return Iterator(m_head);
}

template<typename T, typename Allocator>
typename List<T, Allocator>::Iterator& List<T, Allocator>::end() {
    return Iterator(m_tail->next);
}

template<typename T, typename Allocator>
const typename List<T, Allocator>::Iterator& List<T, Allocator>::cend() const {
    return Iterator(m_tail->next);
}

//List

template<typename T, typename Allocator>
List<T, Allocator>::List() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

template<typename T, typename Allocator>
List<T, Allocator>::List(const List<T, Allocator>& other) : m_head(other.m_head),
                                                            m_tail(other.m_tail), m_size(other.m_size) {}

template<typename T, typename Allocator>
List<T, Allocator>::List(List<T, Allocator>&& other) noexcept: m_head(other.m_head),
                                                            m_tail(other.m_tail), m_size(other.m_size) {
    other.m_head = nullptr;
    other.m_tail = nullptr;
    other.m_size = 0;
}

template<typename T, typename Allocator>
void List<T, Allocator>::deleter() noexcept {
    while (m_head) {
        ListNode* temp = m_head;
        alloc.destroy(&temp->val);
        m_head = m_head->next;
        alloc.deallocate(temp, 1);
    }
    m_tail = nullptr;
}

template<typename T, typename Allocator>
List<T, Allocator>::~List() noexcept {
    deleter();
}

template<typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(const List<T, Allocator>& other) {
    if (this != &other) {
        this->deleter();
        m_head = other.m_head;
        m_tail = other.m_tail;
        m_size = other.m_size;
    }
    return *this;
}

template<typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(List<T, Allocator>&& other) noexcept{
    if (this != &other) {
        this->deleter();
        m_head = other.m_head;
        m_tail = other.m_tail;
        m_size = other.m_size;
        other.m_head = nullptr;
        other.m_tail = nullptr;
        other.m_size = 0;
    }
    return *this;
}

template<typename T, typename Allocator>
[[nodiscard]] bool List<T, Allocator>::empty() const noexcept {
    return !m_size;
}

template<typename T, typename Allocator>
[[nodiscard]] bool List<T, Allocator>::find(const T& in_val) const noexcept {
    for (auto it = begin(); it != end(); it++)
        if (*it == in_val)
            return true;

    return false;
}

template<typename T, typename Allocator>
T& List<T, Allocator>::back() const {
    if (!m_tail)
        throw std::out_of_range("List is empty");

    return m_tail->val;
}

template<typename T, typename Allocator>
T& List<T, Allocator>::front() const {
    if (!m_head)
        throw std::out_of_range("List is empty");

    return m_head->val;
}

template<typename T, typename Allocator>
void List<T, Allocator>::reverse() noexcept {
    auto* tempHead = m_head;

    while (tempHead) {
        auto* tmpNext = tempHead->next;
        tempHead->next = tempHead->prev;
        tempHead->prev = tmpNext;
        tempHead = tempHead->prev;
    }

    std::swap(m_tail, m_head);
}

template<typename T, typename Allocator>
void List<T, Allocator>::swap(List<T, Allocator>& other) noexcept {
    std::swap(m_head, other.m_head);
    std::swap(m_tail, other.m_tail);
    std::swap(m_size, other.m_size);
}

template<typename T, typename Allocator>
Allocator& List<T, Allocator>::getAllocator() const noexcept {
    return alloc;
}

template<typename T, typename Allocator>
void List<T, Allocator>::pop(const size_t& pos) {
   if (pos >= m_size)
       return;

   auto* curr = m_head;

   for (auto i = 0; i < pos; i++)
       curr = curr->next;

   auto* tmpPrev = curr->prev;
   auto* tmpNext = curr->next;

   alloc.destroy(&curr->val);
   alloc.deallocate(curr, 1);
   curr = nullptr;

   tmpPrev->next = tmpNext;
   tmpNext->prev = tmpPrev;
   m_size--;
}

template<typename T, typename Allocator>
void List<T, Allocator>::pop_back() noexcept {
    if (m_tail) {
        auto *temp = m_tail;

        if (m_head == m_tail)
            m_head = nullptr;
        else {
            auto* current = m_tail->prev;
            current->next = nullptr;
        }

        m_tail = nullptr;
        alloc.destroy(&temp->val);
        alloc.deallocate(temp, 1);
        m_size--;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::pop_front() noexcept {
    if (m_head) {
        auto *temp = m_head;

        if (m_head == m_tail)
            m_tail = nullptr;
        else {
            auto* current = m_head->next;
            current->prev = nullptr;
        }

        m_head = nullptr;
        alloc.destroy(&temp->val);
        alloc.deallocate(temp, 1);
        m_size--;
    }
}


template<typename T, typename Allocator>
void List<T, Allocator>::insert(const size_t& pos, const T& in_val) {
    if (pos >= m_size)
        return;

    auto* newNode = alloc.allocate(1);
    alloc.construct(newNode, in_val);

    auto* curr = m_head;

    for (auto i = 0; i < pos - 1; i++)
        curr = curr->next;

    auto* tmpNext = curr->next;
    curr->next = newNode;
    newNode->next = tmpNext;
    newNode->prev = curr;
    m_size++;
}

template<typename T, typename Allocator>
void List<T, Allocator>::push_back(const T& in_val) {
    auto* newTail = alloc.allocate(1);
    alloc.construct(newTail, in_val);

    if (m_tail) {
        newTail->prev = m_tail;
        m_tail->next = newTail;
        m_tail = m_tail->next;
    }
    else {
        m_tail = newTail;
        m_head = newTail;
    }
    m_size++;
}

template<typename T, typename Allocator>
void List<T, Allocator>::push_front(const T& in_val) {
    auto* newHead = alloc.allocate(1);
    alloc.construct(newHead, in_val);
    if (m_head) {
        newHead->next = m_head;
        m_head->prev = newHead;
        m_head = m_head->prev;
    }
    else {
        m_tail = newHead;
        m_head = newHead;
    }
    m_size++;
}

template<typename T, typename Allocator>
[[nodiscard]] size_t List<T, Allocator>::size() const noexcept {
    return m_size;
}

template<typename T, typename Allocator>
[[nodiscard]] size_t List<T, Allocator>::max_size() const noexcept {
    return std::numeric_limits<size_t>::max() / sizeof(ListNode);
}

#endif //LIST_LIST_H
