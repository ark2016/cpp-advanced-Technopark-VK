// test_lru.cpp
#include <bits/stdc++.h>
#include "lru.hpp"
#include <cassert>

int main() {
    // Create a cache with max 2 entries and 100 bytes
    LRUCache<std::string, std::vector<float>> cache(2, 100);

    // Test insertion
    std::vector<float> embed1 = {1.0f, 2.0f, 3.0f};
    size_t size1 = 5 + embed1.size() * sizeof(float); // "key1" + embed
    assert(cache.put("key1", embed1, size1));
    assert(cache.size() == 1);
    assert(cache.size_bytes() == size1);

    std::vector<float> embed2 = {4.0f, 5.0f};
    size_t size2 = 5 + embed2.size() * sizeof(float);
    assert(cache.put("key2", embed2, size2));
    assert(cache.size() == 2);
    assert(cache.size_bytes() == size1 + size2);

    // Test retrieval
    std::vector<float> retrieved;
    assert(cache.get("key1", retrieved));
    assert(retrieved == embed1);

    // Test LRU order by adding a new entry which causes eviction
    std::vector<float> embed3 = {6.0f};
    size_t size3 = 5 + embed3.size() * sizeof(float);
    assert(cache.put("key3", embed3, size3)); // Should evict key2
    assert(cache.size() == 2);
    assert(cache.size_bytes() == size1 + size3);
    assert(!cache.exists("key2"));
    assert(cache.exists("key1"));
    assert(cache.exists("key3"));

    // Test updating existing entry
    std::vector<float> embed1_new = {7.0f, 8.0f};
    size_t size1_new = 5 + embed1_new.size() * sizeof(float);
    assert(cache.put("key1", embed1_new, size1_new));
    assert(cache.size() == 2);
    assert(cache.size_bytes() == size1_new + size3);
    assert(cache.get("key1", retrieved));
    assert(retrieved == embed1_new);

    // Test eviction when size exceeds
    std::vector<float> embed4 = std::vector<float>(50, 9.0f); // Large embed
    size_t size4 = 5 + embed4.size() * sizeof(float);
    assert(!cache.put("key4", embed4, size4)); // Should not insert
    assert(cache.size() == 2);
    assert(!cache.exists("key4"));

    // Clear cache
    cache.clear();
    assert(cache.empty());

    std::cout << "All LRUCache tests passed!\n";
    return 0;
}