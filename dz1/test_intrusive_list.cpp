#include "intrusive_list.hpp"
#include <cassert>
#include <iostream>

struct TestNode : public IntrusiveListNode<TestNode> {
    int value;

    TestNode(int v) : value(v) {}
};

int main() {
    IntrusiveList<TestNode> list;

    // Test pushing to front
    TestNode node1(1);
    TestNode node2(2);
    TestNode node3(3);

    list.push_front(node1);
    assert(list.size() == 1);
    assert(list.get_back()->value == 1);
    assert(list.begin()->value == 1);

    list.push_front(node2);
    assert(list.size() == 2);
    assert(list.get_back()->value == 1);
    assert(list.begin()->value == 2);

    list.push_front(node3);
    assert(list.size() == 3);
    assert(list.get_back()->value == 1);
    assert(list.begin()->value == 3);

    // Test moving to front
    list.move_to_front(node1);
    assert(list.begin()->value == 1);
    assert(list.begin()->next->value == 3);
    assert(list.begin()->next->next->value == 2);

    // Test popping from back
    list.pop_back();
    assert(list.size() == 2);
    assert(list.get_back()->value == 3);

    list.pop_back();
    assert(list.size() == 1);
    assert(list.get_back()->value == 1);

    list.pop_back();
    assert(list.size() == 0);
    assert(list.empty());

    // Test iterator
    list.push_front(node1);
    list.push_front(node2);
    list.push_front(node3);

    auto it = list.begin();
    assert(it->value == 3);
    ++it;
    assert(it->value == 2);
    ++it;
    assert(it->value == 1);
    ++it;
    assert(it == list.end());

    //--it;
    //assert(it->value == 1);
    //--it;
    //assert(it->value == 2);
    //--it;
    //assert(it->value == 3);

    // Test const iterator
    const auto& const_list = list;
    auto cit = const_list.begin();
    assert(cit->value == 3);
    ++cit;
    assert(cit->value == 2);
    ++cit;
    assert(cit->value == 1);
    ++cit;
    assert(cit == const_list.end());

    //--cit;
    //assert(cit->value == 1);
    //--cit;
    //assert(cit->value == 2);
    //--cit;
    //assert(cit->value == 3);

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
