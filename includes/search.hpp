#ifndef search_lib
#define search_lib

#include <iostream>
#include <vector>
#include "./trie.hpp"
#include "./hash.hpp"
#include "./heap.hpp"

void search_player(string player, Tst &tst_players, HashMap<int, Player> &hash_players) {
    // Implementação - Guillermo
    cout << player << endl;
}

void search_user(int user_id, HashMap<int, HeapMin> &hash_users, HashMap<int, Player> hash_players) {
    // Implementação - Guillermo
    cout << user_id << endl;
}

void search_top_n(int n_players, string position, vector<Review> &top_players, HashMap<int, Player> &hash_players) {
    // Implementação - Víctor
    cout << n_players << " " << position << endl;
}

void search_tags(vector<string> &tags, HashMap<string, HashMap<int, int>> hash_tags, HashMap<int, Player> hash_players) {
    // Implementação - Víctor
    for(string tag : tags) {
        cout << tag << " ";
    }
    cout << endl;
}

#endif // search_lib