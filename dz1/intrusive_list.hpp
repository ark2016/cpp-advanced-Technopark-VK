// intrusive_list.hpp
#ifndef INTRUSIVE_LIST_HPP
#define INTRUSIVE_LIST_HPP

#include <iterator>
#include <cstddef>
#include <cassert>

template <typename T>
struct IntrusiveListNode {
    T* prev = nullptr;
    T* next = nullptr;
};

template <typename T>
class IntrusiveList {
private:
    T* head;
    T* tail;
    size_t list_size;

public:
    IntrusiveList() : head(nullptr), tail(nullptr), list_size(0) {}

    ~IntrusiveList() {
        clear();
    }

    IntrusiveList(const IntrusiveList&) = delete;
    IntrusiveList& operator=(const IntrusiveList&) = delete;

    void push_front(T& node) {
        node.next = head;
        node.prev = nullptr;
        if (head)
            head->prev = &node;
        head = &node;
        if (!tail)
            tail = head;
        ++list_size;
    }

    void move_to_front(T& node) {
        if (&node == head) return;
        // Remove node
        if (node.prev)
            node.prev->next = node.next;
        if (node.next)
            node.next->prev = node.prev;
        if (&node == tail)
            tail = node.prev;
        // Insert at front
        node.next = head;
        node.prev = nullptr;
        if (head)
            head->prev = &node;
        head = &node;
    }

    void pop_back() {
        if (!tail) return;
        T* old_tail = tail;
        tail = tail->prev;
        if (tail)
            tail->next = nullptr;
        else
            head = nullptr;
        old_tail->prev = nullptr;
        old_tail->next = nullptr;
        --list_size;
    }

    T* get_back() const {
        return tail;
    }

    bool empty() const {
        return list_size == 0;
    }

    size_t size() const {
        return list_size;
    }

    void clear() {
        head = nullptr;
        tail = nullptr;
        list_size = 0;
    }

    // Iterator implementation
    class iterator {
    private:
        T* current;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(T* node) : current(node) {}

        reference operator*() const { return *current; }
        pointer operator->() const { return current; }

        iterator& operator++() {
            current = current->next;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            current = current->next;
            return tmp;
        }

        iterator& operator--() {
            current = current->prev;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            current = current->prev;
            return tmp;
        }

        bool operator==(const iterator& other) const { return current == other.current; }
        bool operator!=(const iterator& other) const { return current != other.current; }
    };

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }

    // Const iterator
    class const_iterator {
    private:
        const T* current;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator(const T* node) : current(node) {}

        reference operator*() const { return *current; }
        pointer operator->() const { return current; }

        const_iterator& operator++() {
            current = current->next;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            current = current->next;
            return tmp;
        }

        const_iterator& operator--() {
            current = current->prev;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator tmp = *this;
            current = current->prev;
            return tmp;
        }

        bool operator==(const const_iterator& other) const { return current == other.current; }
        bool operator!=(const const_iterator& other) const { return current != other.current; }
    };

    const_iterator begin() const { return const_iterator(head); }
    const_iterator end() const { return const_iterator(nullptr); }
};

#endif // INTRUSIVE_LIST_HPP
