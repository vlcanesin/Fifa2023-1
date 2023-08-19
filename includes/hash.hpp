#ifndef hash_lib
#define hash_lib

#include <iostream>
#include <vector>
#include <string>
#include <functional>

using namespace std;

template<typename K>
struct Hash {
    size_t operator()(K& key, size_t table_size);
};

// Template specialization for string
template<>
struct Hash<string> {
    size_t operator()(string &key, size_t table_size) {
        size_t hash_value = 0;
        for (char c : key) {
            hash_value = (hash_value * 31) + static_cast<size_t>(c);
            hash_value = hash_value % table_size;
        }
        return hash_value % table_size;
    }
};

// Template specialization for int
template<>
struct Hash<int> {
    size_t operator()(int key, size_t table_size) {
        return static_cast<size_t>(key) % table_size;
    }
};

template<typename K, typename V>
struct HashNode {
    K key;
    V value;
    HashNode() : key(K()), value(V()) {}
    HashNode(const K& k, const V& v) : key(k), value(v) {}
};

struct Player {
    string name;
    size_t n_reviews;
    size_t sum_reviews_x2;
    vector<string> positions;

    Player() : name(""), n_reviews(0), sum_reviews_x2(0), positions(vector<string>()) {}
    Player(string name_, size_t n_reviews_, size_t sum_reviews_x2_, vector<string> positions_)
        : name(name_), n_reviews(n_reviews_), sum_reviews_x2(sum_reviews_x2_), positions(positions_) {}

};

template <typename K, typename V>
class HashMap {
private:
    vector<vector<HashNode<K, V>>> table;
    Hash<K> hasher;
    size_t table_size;
    size_t n_elements;

public:
    HashMap() : table_size(100), table(100, vector<HashNode<K, V>>()), n_elements(0) {}  // default: hash for the tags
    HashMap(size_t initial_size) : table_size(initial_size), table(initial_size, vector<HashNode<K, V>>()), n_elements(0) {}

    class Iterator {
    private:
        typename vector<vector<HashNode<K, V>>>::iterator outer_iter;
        typename vector<HashNode<K, V>>::iterator inner_iter;
        HashMap<K, V>& hash_map; // ***Reference*** to the hash map instance
    
    public:
        Iterator(const typename vector<vector<HashNode<K, V>>>::iterator outer, 
                 const typename vector<HashNode<K, V>>::iterator inner,
                 HashMap<K, V>& hm)
            : outer_iter(outer), inner_iter(inner), hash_map(hm) {}

        bool operator==(const Iterator& comp_it) const {
            return (outer_iter == comp_it.outer_iter) && (inner_iter == comp_it.inner_iter);
        }
        bool operator!=(const Iterator& comp_it) const {
            return !(*this == comp_it);
        }
        Iterator& operator++() {
            inner_iter++;
            // If inner_iter got to the end of the hash list
            while (outer_iter != hash_map.table.end() && inner_iter == outer_iter->end()) {
                // Keeps searching for the next non-empty hash list
                outer_iter++;
                if(outer_iter != hash_map.table.end()) {
                    inner_iter = outer_iter->begin();
                }
            }
            if(outer_iter != hash_map.table.end()) return *this;
            else {
                inner_iter = typename vector<HashNode<K, V>>::iterator();   // the same way as in the .end() function
                return *this;
            }
        }
        HashNode<K, V>& operator*() {
            return *inner_iter;
        }
    };

    // ------------------------------ ITERATOR METHODS:
    Iterator begin() {
        typename vector<vector<HashNode<K, V>>>::iterator outer = table.begin();
        // Begins the search at the first element
        while(outer != table.end() && outer->empty()) {
            // If the current hash list is empty, goes to the next
            ++outer;
        }
        if(outer == table.end()) return end();
        else return Iterator(outer, outer->begin(), *this);
    }
    Iterator end() {
        return Iterator(table.end(), typename vector<HashNode<K, V>>::iterator(), *this);
    }
    // Experimental: returns the iterator pointing to the last empty bucket 
    // (from .end() to .beg()). This would prevent the searches to happen
    // in empty buckets that come after the last non-empty bucket.
    Iterator fast_end() {
        typename vector<vector<HashNode<K, V>>>::iterator outer = table.end()-1;
        // Begins the search at the last element
        do {
            // If the current hash list is empty, goes to the one prior
            if(outer->empty()) {
                if(outer != table.begin()) {
                    --outer;
                }
            } else break;
        } while(outer != table.begin());
        // Note: the .end() iter points to the first element thats out of
        // the scope of the container
        if(outer != table.begin() || !outer->empty()) ++outer;
        if(outer == table.end()) return Iterator(outer, typename vector<HashNode<K, V>>::iterator(), *this);
        else return Iterator(outer, outer->begin(), *this);
    }
    bool empty() {
        return this->begin() == this->end();
    }

    size_t size() {
        return n_elements;
    }

    // This function returns a reference to a node of the HashTable given its id
    // If the node is not present, then an empty element of type V is inserted in the table
    V& get(K &key_to_find) {
        size_t hashed_id = hasher(key_to_find, table_size);
        for(HashNode<K, V> &curr_node : table[hashed_id]) {
            if(key_to_find == curr_node.key) {
                return curr_node.value;
            }
        }
        table[hashed_id].push_back(HashNode<K, V>(key_to_find, V()));
        n_elements++;
        return table[hashed_id].back().value;
    }

    bool has_key(K &key_to_find) {
        size_t hashed_id = hasher(key_to_find, table_size);
        for(HashNode<K, V> &curr_node : table[hashed_id]) {
            if(key_to_find == curr_node.key) {
                return true;
            }
        }
        return false;
    }

};

#endif // hash_lib
