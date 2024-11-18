// test_intrusive_list.cpp

#include "intrusive_list.hpp"
#include <cassert>
#include <iostream>

struct MyNode : public ListNode<MyNode> {
    int value;

    MyNode(int val) : value(val) {}
};

void test_push_pop() {
    IntrusiveList<MyNode> list;
    MyNode n1(1), n2(2), n3(3);

    list.push_back(n1);
    assert(list.size() == 1);
    assert(&list.front() == &n1);
    assert(&list.back() == &n1);

    list.push_back(n2);
    assert(list.size() == 2);
    assert(&list.front() == &n1);
    assert(&list.back() == &n2);

    list.push_front(n3);
    assert(list.size() == 3);
    assert(&list.front() == &n3);
    assert(&list.back() == &n2);

    list.pop_front();
    assert(list.size() == 2);
    assert(&list.front() == &n1);

    list.pop_back();
    assert(list.size() == 1);
    assert(&list.back() == &n1);

    list.pop_back();
    assert(list.empty());

    std::cout << "test_push_pop passed\n";
}

void test_insert_erase() {
    IntrusiveList<MyNode> list;
    MyNode n1(1), n2(2), n3(3), n4(4);

    list.push_back(n1);
    list.push_back(n2);

    auto it = list.begin();
    ++it;
    list.insert(it, n3);

    assert(list.size() == 3);
    it = list.begin();
    assert(&*it == &n1);
    ++it;
    assert(&*it == &n3);
    ++it;
    assert(&*it == &n2);

    list.erase(--list.end());

    assert(list.size() == 2);
    assert(&list.back() == &n3);

    list.erase(list.begin());

    assert(list.size() == 1);
    assert(&list.front() == &n3);

    list.erase(list.begin());
    assert(list.empty());

    std::cout << "test_insert_erase passed\n";
}

void test_splice() {
    IntrusiveList<MyNode> list1, list2;
    MyNode n1(1), n2(2), n3(3), n4(4);

    list1.push_back(n1);
    list1.push_back(n2);
    list2.push_back(n3);
    list2.push_back(n4);

    list1.splice(list1.end(), list2, list2.begin(), list2.end());

    assert(list1.size() == 4);
    assert(list2.empty());

    auto it = list1.begin();
    assert(&*it == &n1);
    ++it;
    assert(&*it == &n2);
    ++it;
    assert(&*it == &n3);
    ++it;
    assert(&*it == &n4);

    std::cout << "test_splice passed\n";
}

void test_iterators() {
    IntrusiveList<MyNode> list;
    MyNode n1(1), n2(2), n3(3);

    list.push_back(n1);
    list.push_back(n2);
    list.push_back(n3);

    auto it = list.begin();
    assert(&*it == &n1);
    ++it;
    assert(&*it == &n2);
    ++it;
    assert(&*it == &n3);
    ++it;
    assert(it == list.end());

    --it;
    assert(&*it == &n3);
    --it;
    assert(&*it == &n2);
    --it;
    assert(&*it == &n1);
    assert(it == list.begin());

    std::cout << "test_iterators passed\n";
}

void test_move_constructor() {
    IntrusiveList<MyNode> list1;
    MyNode n1(1), n2(2);

    list1.push_back(n1);
    list1.push_back(n2);

    IntrusiveList<MyNode> list2(std::move(list1));
    assert(list2.size() == 2);
    assert(list1.empty());

    std::cout << "test_move_constructor passed\n";
}

void test_assignment_operator() {
    IntrusiveList<MyNode> list1;
    MyNode n1(1), n2(2);

    list1.push_back(n1);
    list1.push_back(n2);

    IntrusiveList<MyNode> list2;
    list2 = std::move(list1);
    assert(list2.size() == 2);
    assert(list1.empty());

    std::cout << "test_assignment_operator passed\n";
}

void test_clear() {
    IntrusiveList<MyNode> list;
    MyNode n1(1), n2(2);

    list.push_back(n1);
    list.push_back(n2);

    list.clear();

    assert(list.empty());
    assert(n1.next == nullptr && n1.prev == nullptr);
    assert(n2.next == nullptr && n2.prev == nullptr);

    std::cout << "test_clear passed\n";
}

int main() {
    test_push_pop();
    test_insert_erase();
    test_splice();
    test_iterators();
    test_move_constructor();
    test_assignment_operator();
    test_clear();

    std::cout << "All tests passed\n";
    return 0;
}
