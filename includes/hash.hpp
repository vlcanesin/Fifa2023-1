#ifndef hash_lib
#define hash_lib

#include <iostream>
#include <vector>
#include <string>

#define MAX_ELEMENTS_PER_LIST 1000000

using namespace std;

// Structs used for the Hash Table:

typedef struct data {
    string name;
    string positions;
}Data;

typedef struct player {
    int id;
    Data data;
}Player;

typedef struct node {
    Player player;
    struct node *next;
}HashNode;

typedef struct list {
    HashNode *begin, *end;
    int size;
}HashList;

typedef vector<HashList> HashTable;

// Structs used for statistics:

typedef struct hashstats {
    int n_lists_used;
    int min_size, max_size;
    float avg_size;
}HashStats;

typedef struct searchstats{
    string name;
    int n_tests;
    bool found;
}SearchStats;

// Functions:

int Hash(int id, int M);
// Hashing function

void InitializeHashTable(HashTable &table, int M);
// Initialize a hash table with size M

void Insert(Player p, HashTable &table);
// Insert element in hash table

HashStats CalculateHashStats(HashTable table);
SearchStats SearchPlayer(int id, HashTable table);
// Statistics for measuring performance

#endif // hash_lib
