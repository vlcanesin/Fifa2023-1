#ifndef hash_lib
#define hash_lib

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Forward declarations

class IdClass;
class IntId;
class StringId;
class NodeClass;
class PlayerNode;

// CLASSES USED FOR HASH ID TYPES (int or string)

class IdClass {
public:
    virtual ~IdClass() {}
    virtual int hash(int M) = 0;
};

class IntId : public IdClass {
public:
    int value;

    IntId() : value(0) {}
    IntId(int initialId) : value(initialId) {}

    int hash(int M) override {
        return value%M;
    }
};

class StringId : public IdClass {
public:
    string value;

    //IntId() : value("") {}
    StringId(string initialId) : value(initialId) {}

    int hash(int M) override {
        int ans, p = 257;
        for(char c : value) {
            ans = (ans + int(c)*p) % M;
            p = (p*257) % M;
        }
        return ans;
    }
};

// CLASSES USED FOR NODE TYPES (int, heap and the HashTable itself)

class NodeClass {
public: 
    virtual ~NodeClass() {}
    virtual void update(NodeClass &element) {}
    virtual void updatePlayer(PlayerNode *element) {}
};

class PlayerNode : public NodeClass {
public:
    IntId id;
    string name;
    unsigned long long n_reviews;
    unsigned long long sum_reviews_x2;

    PlayerNode() : id(0), name(""), n_reviews(0), sum_reviews_x2(0) {}

    PlayerNode(int id_, string name_) : id(id_), name(name_), n_reviews(0), sum_reviews_x2(0) {}

    PlayerNode(int id_, string name_, unsigned long long n_reviews_, unsigned long long sum_reviews_x2_)
        : id(id_), name(name_), n_reviews(n_reviews_), sum_reviews_x2(sum_reviews_x2_) {}

    // This function is called when the element to insert is already in the hash table.
    // It only updates n_reviews and sum_reviews_x2
    void update(NodeClass &element) override {
        element.updatePlayer(this);
    }
    void updatePlayer(PlayerNode *element) override {
        element->n_reviews += n_reviews;
        element->sum_reviews_x2 += sum_reviews_x2;
    }

};

template <class NodeType>
class HashTable : private vector<vector<NodeType>>, public NodeClass {
private:
    typedef vector<vector<NodeType>> base_vector;
    int M;

public:
    typedef typename base_vector::size_type       size_type;
    typedef typename base_vector::iterator        iterator;
    typedef typename base_vector::const_iterator  const_iterator;

    using base_vector::operator[];
    using base_vector::begin;
    using base_vector::clear;
    using base_vector::end;
    using base_vector::erase;
    using base_vector::push_back;
    using base_vector::reserve;
    using base_vector::resize;
    using base_vector::size;

    HashTable() {
        M = 10000;
        this->assign(M, vector<NodeType>());
    }
    HashTable(int initialM) {
        M = initialM;
        this->assign(M, vector<NodeType>());
    }

    // Inserts a node in the hash table.
    // If it is already present, then the update function of the node object
    // is called in order to update its content accordingly
    void insert(NodeType node) {
        int hashed_id = node.id.hash(M);
        bool already = false;
        for(auto TableNodeIt = (*this)[hashed_id].begin(); TableNodeIt != (*this)[hashed_id].end(); TableNodeIt++) {
            if(TableNodeIt->id.value == node.id.value) {
                TableNodeIt->update(node);
                already = true;
                break;
            }
        }
        if(!already) {
            (*this)[hashed_id].push_back(node);
        }
    }
    
};

#endif // hash_lib
