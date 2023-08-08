#include <iostream>
#include <vector>
#include <string>
#include "./includes/hash.hpp"

using namespace std;

int Hash(int id, int M) {
    return id%M;
}

void InitializeHashTable(HashTable &table, int M) {
    table.clear();
    HashList default_list = (HashList){NULL, NULL, 0};
    table.assign(M, default_list);
}

void Insert(Player p, HashTable &table) {
    
    int id = Hash(p.id, table.size());
    
    HashNode *newNode = new HashNode;
    newNode->player = p;
    newNode->next = NULL;
    
    if(table[id].begin == NULL) {
        table[id].begin = newNode;
        table[id].end = newNode;
    } else {
        table[id].end->next = newNode;
        table[id].end = newNode;
    }

    table[id].size++;

}

HashStats CalculateHashStats(HashTable table) {
    HashStats hstats = {0, MAX_ELEMENTS_PER_LIST, 0, 0.0};
    int acc_size = 0;
    int M = table.size();
    for(int id = 0; id < M; id++) {
        int list_size = table[id].size;
        if(list_size > 0) {
            hstats.n_lists_used++;
            hstats.min_size = min(hstats.min_size, list_size);
            hstats.max_size = max(hstats.max_size, list_size);
            acc_size += list_size;
        }
    }
    hstats.avg_size = (float)acc_size / hstats.n_lists_used;
    return hstats;
}

SearchStats SearchPlayer(int id_to_find, HashTable table) {
    SearchStats sstats = {"MISS", 0, false};
    int id = Hash(id_to_find, table.size());
    for(HashNode *ptNode = table[id].begin; ptNode != NULL; ptNode = ptNode->next) {
        sstats.n_tests++;
        if(ptNode->player.id == id_to_find) {
            sstats.name = ptNode->player.data.name;
            sstats.found = true;
            break;
        }
    }
    return sstats;
}
