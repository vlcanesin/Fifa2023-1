#ifndef hash_lib
#define hash_lib

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Forward declarations

//class HashContent;
//class HashNode;
//class HashMap;
//class Player;

// HASH: has the hash function implementation, which uses
// the bytes of the key in order to calculate the hashed id.

template<typename K>
struct Hash {
    size_t operator()(K& key, size_t table_size) {
        size_t hash_value = 0, p = 257;
        char* key_bytes = reinterpret_cast<char*>(&key);    
        // key_bytes is a char pointer that is used to access the bytes of "key"
        for(size_t i = 0; i < sizeof(key_bytes); i++) {
            hash_value = (hash_value + size_t(key_bytes[i])*p) % table_size;
            p = (p*257) % table_size;
        }
        return hash_value;
    }
};

template<typename K, typename V>
struct HashNode {
    K key;
    V value;
    HashNode() : key(K()), value(V()) {}
    HashNode(K& k, V& v) : key(k), value(v) {}
};

class Player {
public:
    string name;
    size_t n_reviews;
    size_t sum_reviews_x2;

    Player() : name(""), n_reviews(0), sum_reviews_x2(0) {}
    Player(string name_, size_t n_reviews_, size_t sum_reviews_x2_)
        : name(name_), n_reviews(n_reviews_), sum_reviews_x2(sum_reviews_x2_) {}

    // This function is called when the element to insert is already in the hash table.
    // It only updates n_reviews and sum_reviews_x2
    void update(Player &incoming_player) {
        n_reviews += incoming_player.n_reviews;
        sum_reviews_x2 += incoming_player.sum_reviews_x2;
    }

};

template <typename K, typename V>
class HashMap {
private:
    vector<vector<HashNode<K, V>>> table;
    Hash<K> hasher;
    size_t table_size;

public:
    HashMap() : table_size(1000), table(1000, vector<HashNode<K, V>>()) {}
    HashMap(size_t initial_size) : table_size(initial_size), table(initial_size, vector<HashNode<K, V>>()) {}

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
    // in empty buckets that come after the bucket returned by this function.
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

    // Inserts a node in the hash table.
    // If it is already present, then the update function of the node object
    // is called in order to update its content accordingly
    void insert(HashNode<K, V> &node) {
        size_t hashed_id = hasher(node.key, table_size);
        for(HashNode<K, V> &curr_node : table[hashed_id]) {
            if(node.key == curr_node.key) {
                curr_node.value.update(node.value);
                return;
            }
        }
        table[hashed_id].push_back(node);
    }

    // This function returns a reference to a node of the HashTable given its id
    // If the node is not present, then an empty element of type V is inserted in the table
    HashMap<K, V>& get(K &key_to_find) {
        size_t hashed_id = hasher(key_to_find, table_size);
        for(HashNode<K, V> &curr_node : table[hashed_id]) {
            if(key_to_find == curr_node.key) {
                return curr_node.value;
            }
        }
        table[hashed_id].push_back(HashNode<K, V>(key_to_find, V()));
        return table[hashed_id].back().value;
    }

};

#endif // hash_lib
