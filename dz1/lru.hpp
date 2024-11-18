// lru.hpp

#pragma once

#include <unordered_map>
#include "intrusive_list.hpp"

#include <cstddef>
#include <functional>
#include <utility>

template <
    typename Key,
    typename Value,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>
>
class LRUCache {
private:
    struct Node : public ListNode<Node> {
        Key key;
        Value value;
        std::size_t key_size;
        std::size_t value_size;

        Node(const Key& k, const Value& v, std::size_t k_size, std::size_t v_size)
            : key(k), value(v), key_size(k_size), value_size(v_size) {}
    };

    std::unordered_map<Key, Node*, Hash, KeyEqual> cache_map;
    IntrusiveList<Node> cache_list;
    std::size_t capacity;
    std::size_t byte_capacity;
    std::size_t current_size;
    std::size_t current_byte_size;

public:
    LRUCache(std::size_t capacity, std::size_t byte_capacity)
        : capacity(capacity), byte_capacity(byte_capacity), current_size(0), current_byte_size(0) {}

    ~LRUCache() {
        clear();
    }

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

    bool put(const Key& key, const Value& value, std::size_t key_size, std::size_t value_size) {
        if (key_size + value_size > byte_capacity || capacity == 0) {
            return false;
        }

        auto it = cache_map.find(key);
        if (it != cache_map.end()) {
            Node* node = it->second;
            current_byte_size -= node->key_size + node->value_size;
            node->value = value;
            node->key_size = key_size;
            node->value_size = value_size;
            current_byte_size += key_size + value_size;
            cache_list.erase(cache_list.iterator(node));
            cache_list.push_front(*node);
        } else {
            while ( (current_size >= capacity) || (current_byte_size + key_size + value_size > byte_capacity) ) {
                if (cache_list.empty()) {
                    return false;
                }
                Node& old_node = cache_list.back();
                cache_map.erase(old_node.key);
                current_byte_size -= old_node.key_size + old_node.value_size;
                cache_list.pop_back();
                delete &old_node;
                --current_size;
            }
            Node* node = new Node(key, value, key_size, value_size);
            cache_list.push_front(*node);
            cache_map[key] = node;
            current_byte_size += key_size + value_size;
            ++current_size;
        }

        return true;
    }

    bool get(const Key& key, Value& value) {
        auto it = cache_map.find(key);
        if (it == cache_map.end()) {
            return false;
        }
        Node* node = it->second;
        cache_list.erase(cache_list.iterator(node));
        cache_list.push_front(*node);
        value = node->value;
        return true;
    }

    void clear() {
        for (auto& pair : cache_map) {
            delete pair.second;
        }
        cache_map.clear();
        cache_list.clear();
        current_size = 0;
        current_byte_size = 0;
    }

    std::size_t size() const {
        return current_size;
    }

    std::size_t size_bytes() const {
        return current_byte_size;
    }
};
