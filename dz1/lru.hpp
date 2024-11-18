// lru.hpp
#ifndef LRU_HPP
#define LRU_HPP

#include <unordered_map>
#include <list>
#include <vector>
#include <string>
#include <cstddef>
#include <memory>
#include <cstring>
#include "intrusive_list.hpp"

template <typename Key, typename Value>
class LRUCache {
private:
    struct CacheEntry : public IntrusiveListNode<CacheEntry> {
        Key key;
        Value value;
        size_t size_bytes;

        CacheEntry(const Key& k, const Value& v, size_t size)
                : key(k), value(v), size_bytes(size) {}
    };

    using ListType = IntrusiveList<CacheEntry>;
    using ListIterator = ListType::iterator;

    size_t max_entries;
    size_t max_size_bytes;
    size_t current_size_bytes;

    ListType list;
    std::unordered_map<Key, CacheEntry*> map;

public:
    LRUCache(size_t max_n, size_t max_b)
            : max_entries(max_n), max_size_bytes(max_b), current_size_bytes(0) {}

    ~LRUCache() {
        clear();
    }

    bool exists(const Key& key) {
        return map.find(key) != map.end();
    }

    bool get(const Key& key, Value& value) {
        auto it = map.find(key);
        if (it == map.end()) {
            return false;
        }
        CacheEntry* entry = it->second;
        list.move_to_front(*entry);
        value = entry->value;
        return true;
    }

    bool put(const Key& key, const Value& value, size_t entry_size) {
        if (entry_size > max_size_bytes || max_entries == 0) {
            return false;
        }

        if (map.find(key) != map.end()) {
            CacheEntry* existing = map[key];
            current_size_bytes -= existing->size_bytes;
            existing->value = value;
            existing->size_bytes = entry_size;
            current_size_bytes += entry_size;
            list.move_to_front(*existing);
        } else {
            // Evict while necessary
            while ((map.size() >= max_entries || current_size_bytes + entry_size > max_size_bytes) && !list.empty()) {
                CacheEntry* old = list.get_back();
                map.erase(old->key);
                current_size_bytes -= old->size_bytes;
                list.pop_back();
                delete old;
            }

            if (map.size() >= max_entries || current_size_bytes + entry_size > max_size_bytes) {
                return false;
            }

            CacheEntry* new_entry = new CacheEntry(key, value, entry_size);
            list.push_front(*new_entry);
            map[key] = new_entry;
            current_size_bytes += entry_size;
        }

        return true;
    }

    void clear() {
        for (auto& pair : map) {
            delete pair.second;
        }
        map.clear();
        list.clear();
        current_size_bytes = 0;
    }

    size_t size() const {
        return map.size();
    }

    size_t size_bytes() const {
        return current_size_bytes;
    }
};

#endif // LRU_HPP