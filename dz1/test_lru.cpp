#include "lru.hpp"
#include <cassert>
#include <iostream>
#include <string>

int main() {
    LRUCache<int, std::string> cache(3, 100);

    // Test putting and getting values
    assert(cache.put(1, "value1", 10));
    assert(cache.put(2, "value2", 20));
    assert(cache.put(3, "value3", 30));

    std::string value;
    assert(cache.get(1, value));
    assert(value == "value1");

    assert(cache.get(2, value));
    assert(value == "value2");

    assert(cache.get(3, value));
    assert(value == "value3");

    // Test eviction by size
    LRUCache<int, std::string> cache_size(3, 50);
    assert(cache_size.put(1, "value1", 10));
    assert(cache_size.put(2, "value2", 20));
    assert(cache_size.put(3, "value3", 30));

    // Adding a new entry that exceeds the size limit
    assert(cache_size.put(4, "value4", 40));

    assert(!cache_size.get(1, value));
    assert(value != "value1");

    assert(!cache_size.get(2, value));
    assert(value != "value2");

    assert(!cache_size.get(3, value));
    assert(value == "value3");

    assert(cache_size.get(4, value));

    // Test eviction by count
    LRUCache<int, std::string> cache_count(2, 100);
    assert(cache_count.put(1, "value1", 10));
    assert(cache_count.put(2, "value2", 20));
    assert(cache_count.put(3, "value3", 30));

    assert(!cache_count.get(1, value));

    assert(cache_count.get(2, value));
    assert(value == "value2");

    assert(cache_count.get(3, value));
    assert(value == "value3");

    // Test updating an existing entry
    LRUCache<int, std::string> cache_update(3, 100);
    assert(cache_update.put(1, "value1", 10));
    assert(cache_update.put(1, "value1_updated", 15));

    assert(cache_update.get(1, value));
    assert(value == "value1_updated");
    assert(cache_update.size_bytes() == 15);

    // Test clearing the cache
    LRUCache<int, std::string> cache_clear(3, 100);
    assert(cache_clear.put(1, "value1", 10));
    assert(cache_clear.put(2, "value2", 20));

    cache_clear.clear();

    assert(cache_clear.size() == 0);
    assert(cache_clear.size_bytes() == 0);

    assert(!cache_clear.get(1, value));
    assert(!cache_clear.get(2, value));

    // Test existence check
    LRUCache<int, std::string> cache_exists(3, 100);
    assert(cache_exists.put(1, "value1", 10));
    assert(cache_exists.exists(1));
    assert(!cache_exists.exists(2));

    std::cout << "All tests passed!" << std::endl;

    return 0;
}

