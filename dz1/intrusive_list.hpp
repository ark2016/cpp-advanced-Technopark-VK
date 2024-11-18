// intrusive_list.hpp

#pragma once

#include <iterator>
#include <cstddef>

template <typename T>
struct ListNode {
    T* next;
    T* prev;

    ListNode() : next(nullptr), prev(nullptr) {}
};

template <typename T>
class IntrusiveList {
public:
    IntrusiveList() : head(nullptr), tail(nullptr), sz(0) {}

    ~IntrusiveList() {
        clear();
    }

    IntrusiveList(const IntrusiveList&) = delete;
    IntrusiveList& operator=(const IntrusiveList&) = delete;

    IntrusiveList(IntrusiveList&& other) : head(other.head), tail(other.tail), sz(other.sz) {
        other.head = nullptr;
        other.tail = nullptr;
        other.sz = 0;
    }

    IntrusiveList& operator=(IntrusiveList&& other) {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            sz = other.sz;
            other.head = nullptr;
            other.tail = nullptr;
            other.sz = 0;
        }
        return *this;
    }

    bool empty() const {
        return sz == 0;
    }

    std::size_t size() const {
        return sz;
    }

    void clear() {
        head = nullptr;
        tail = nullptr;
        sz = 0;
    }

    T& front() {
        return *head;
    }

    const T& front() const {
        return *head;
    }

    T& back() {
        return *tail;
    }

    const T& back() const {
        return *tail;
    }

    void push_front(T& element) {
        insert(begin(), element);
    }

    void push_back(T& element) {
        insert(end(), element);
    }

    void pop_front() {
        erase(begin());
    }

    void pop_back() {
        erase(--end());
    }

    class iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    public:
        iterator(T* node = nullptr) : node(node) {}

        T& operator*() const {
            return *node;
        }

        T* operator->() const {
            return node;
        }

        iterator& operator++() {
            node = node->next;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp(*this);
            node = node->next;
            return tmp;
        }

        iterator& operator--() {
            node = node->prev;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp(*this);
            node = node->prev;
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return node == other.node;
        }

        bool operator!=(const iterator& other) const {
            return node != other.node;
        }

        T* get() const {
            return node;
        }

    private:
        T* node;
    };

    class const_iterator : public std::iterator<std::bidirectional_iterator_tag, const T> {
    public:
        const_iterator(const T* node = nullptr) : node(node) {}

        const T& operator*() const {
            return *node;
        }

        const T* operator->() const {
            return node;
        }

        const_iterator& operator++() {
            node = node->next;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp(*this);
            node = node->next;
            return tmp;
        }

        const_iterator& operator--() {
            node = node->prev;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator tmp(*this);
            node = node->prev;
            return tmp;
        }

        bool operator==(const const_iterator& other) const {
            return node == other.node;
        }

        bool operator!=(const const_iterator& other) const {
            return node != other.node;
        }

        const T* get() const {
            return node;
        }

    private:
        const T* node;
    };

    iterator begin() {
        return iterator(head);
    }

    const_iterator begin() const {
        return const_iterator(head);
    }

    iterator end() {
        return iterator(nullptr);
    }

    const_iterator end() const {
        return const_iterator(nullptr);
    }

    iterator insert(iterator pos, T& element) {
        T* node = &element;
        if (node->next || node->prev || node == head) {
            return iterator(node);
        }

        T* curr = pos.get();

        node->next = curr;
        if (curr) {
            node->prev = curr->prev;
            if (curr->prev) {
                curr->prev->next = node;
            } else {
                head = node;
            }
            curr->prev = node;
        } else {
            node->prev = tail;
            if (tail) {
                tail->next = node;
            } else {
                head = node;
            }
            tail = node;
        }

        ++sz;
        return iterator(node);
    }

    iterator erase(iterator pos) {
        T* node = pos.get();
        if (!node) {
            return end();
        }

        T* next_node = node->next;

        if (node->prev) {
            node->prev->next = node->next;
        } else {
            head = node->next;
        }

        if (node->next) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev;
        }

        node->next = nullptr;
        node->prev = nullptr;

        --sz;
        return iterator(next_node);
    }

    void splice(iterator pos, IntrusiveList& other, iterator first, iterator last) {
        if (first == last) {
            return;
        }

        T* first_node = first.get();
        T* last_node = last.get() ? last.get()->prev : other.tail;

        if (first_node->prev) {
            first_node->prev->next = last.get();
        } else {
            other.head = last.get();
        }

        if (last.get()) {
            last.get()->prev = first_node->prev;
        } else {
            other.tail = first_node->prev;
        }

        T* curr = pos.get();

        if (curr) {
            T* prev_node = curr->prev;
            if (prev_node) {
                prev_node->next = first_node;
            } else {
                head = first_node;
            }
            first_node->prev = prev_node;
            curr->prev = last_node;
            last_node->next = curr;
        } else {
            if (tail) {
                tail->next = first_node;
                first_node->prev = tail;
            } else {
                head = first_node;
                first_node->prev = nullptr;
            }
            tail = last_node;
            last_node->next = nullptr;
        }

        std::size_t n = 0;
        for (T* node = first_node;; node = node->next) {
            ++n;
            if (node == last_node) break;
        }
        other.sz -= n;
        sz += n;
    }

private:
    T* head;
    T* tail;
    std::size_t sz;
};
