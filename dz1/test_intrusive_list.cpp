// test_intrusive_list.cpp
#include <bits/stdc++.h>
#include "intrusive_list.hpp"
#include <cassert>

struct TestNode : public IntrusiveListNode<TestNode> {
    int value;
    TestNode(int val) : value(val) {}
};

int main() {
    IntrusiveList<TestNode> list;
    TestNode n1(1), n2(2), n3(3);

    assert(list.empty());
    list.push_front(n1);
    assert(!list.empty());
    assert(list.size() == 1);
    list.push_front(n2);
    assert(list.size() == 2);
    list.push_front(n3);
    assert(list.size() == 3);

    // Check order
    auto it = list.begin();
    assert(it->value == 3);
    ++it;
    assert(it->value == 2);
    ++it;
    assert(it->value == 1);
    ++it;
    assert(it == list.end());

    // Move n2 to front
    list.move_to_front(n2);
    it = list.begin();
    assert(it->value == 2);
    ++it;
    assert(it->value == 3);
    ++it;
    assert(it->value == 1);
    ++it;
    assert(it == list.end());

    // Pop back
    list.pop_back();
    assert(list.size() == 2);
    it = list.begin();
    assert(it->value == 2);
    ++it;
    assert(it->value == 3);
    ++it;
    assert(it == list.end());

    // Clear
    list.clear();
    assert(list.empty());

    std::cout << "All IntrusiveList tests passed!\n";
    return 0;
}