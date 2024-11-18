// test_lru.cpp

#include "lru.hpp"
#include <cassert>
#include <iostream>
#include <string>

void test_basic() {
    using Key = std::string;
    using Value = std::string;
    LRUCache<Key, Value> cache(3, 100);

    bool res;
    res = cache.put("key1", "value1", 4, 6);
    assert(res);
    res = cache.put("key2", "value2", 4, 6);
    assert(res);
    res = cache.put("key3", "value3", 4, 6);
    assert(res);

    res = cache.put("key4", "value4", 4, 6);
    assert(res);

    Value val;
    res = cache.get("key1", val);
    assert(!res);

    res = cache.get("key2", val);
    assert(res && val == "value2");

    res = cache.get("key3", val);
    assert(res && val == "value3");

    res = cache.get("key4", val);
    assert(res && val == "value4");

    std::cout << "test_basic passed\n";
}

void test_update() {
    using Key = std::string;
    using Value = std::string;
    LRUCache<Key, Value> cache(3, 100);

    bool res;
    res = cache.put("key", "value1", 3, 6);
    assert(res);
    Value val;
    res = cache.get("key", val);
    assert(res && val == "value1");

    res = cache.put("key", "value2", 3, 6);
    assert(res);

    res = cache.get("key", val);
    assert(res && val == "value2");

    std::cout << "test_update passed\n";
}

void test_eviction() {
    using Key = std::string;
    using Value = std::string;
    LRUCache<Key, Value> cache(2, 50);

    bool res;
    res = cache.put("key1", "value1", 4, 6);
    assert(res);
    res = cache.put("key2", "value2", 4, 6);
    assert(res);

    Value val;

    res = cache.get("key1", val);
    assert(res && val == "value1");

    res = cache.put("key3", "value3", 4, 6);
    assert(res);

    res = cache.get("key2", val);
    assert(!res);

    res = cache.get("key1", val);
    assert(res && val == "value1");

    res = cache.get("key3", val);
    assert(res && val == "value3");

    std::cout << "test_eviction passed\n";
}

void test_size_limit() {
    using Key = std::string;
    using Value = std::string;
    LRUCache<Key, Value> cache(10, 20);

    bool res;
    res = cache.put("key1", "value1", 4, 6);
    assert(res);

    res = cache.put("key2", "value2", 4, 6);
    assert(res);

    res = cache.put("key3", "value3", 4, 6);
    assert(!res);

    std::cout << "test_size_limit passed\n";
}

void test_clear() {
    using Key = std::string;
    using Value = std::string;
    LRUCache<Key, Value> cache(3, 100);

    cache.put("key1", "value1", 4, 6);
    cache.put("key2", "value2", 4, 6);

    cache.clear();
    assert(cache.size() == 0);
    assert(cache.size_bytes() == 0);

    Value val;
    bool res = cache.get("key1", val);
    assert(!res);

    std::cout << "test_clear passed\n";
}

int main() {
    test_basic();
    test_update();
    test_eviction();
    test_size_limit();
    test_clear();

    std::cout << "All tests passed\n";
    return 0;
}
