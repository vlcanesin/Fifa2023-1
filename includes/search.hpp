#ifndef search_lib
#define search_lib

#include <iostream>
#include <vector>
#include "./trie.hpp"
#include "./hash.hpp"
#include "./heap.hpp"


void _print_player(int id, Player p) {
    cout << id << ", " << p.name << ", ";
    for(string pos : p.positions) {
        cout << pos << ", ";
    }
    cout << (float)p.sum_reviews_x2/(2*p.n_reviews) << ", " << p.n_reviews << endl;
}


void search_player(string player, Tst &tst_players, HashMap<int, Player> &hash_players) {
     vector <int>ids = tst_players.searchPrefix(player);

    for(auto& id_player: ids){
        Player player_with_prefix = hash_players.get(id_player);
        _print_player(id_player, player_with_prefix);
    }

}

void search_user(int user_id, HashMap<int, HeapMin> &hash_users, HashMap<int, Player> hash_players) {
    HeapMin user_top20 = hash_users.get(user_id);
    cout << user_id << endl;
    user_top20.print();
}



void search_top_n(int n_players, string position, vector<Review> &top_players, HashMap<int, Player> &hash_players) {
    for(Review review : top_players) {
        if(n_players == 0) break;
        Player curr_player = hash_players.get(review.id);
        for(string curr_pos : curr_player.positions) {
            if(curr_pos == position) {
                _print_player(review.id, curr_player);
                n_players--;
                break;
            }
        }
    }
    cout << endl;
}

void search_tags(vector<string> &tags, HashMap<string, HashMap<int, int>> &hash_tags, HashMap<int, Player> &hash_players) {

    //for(string tag : tags) cout << tag << " ";
    //cout << endl;

    int min_size = 1e6;
    string min_tag = "";
    HashMap<int, int> *min_hash = NULL;
    for(string tag : tags) {
        HashMap<int, int> *curr_hash = &hash_tags.get(tag);
        // Obs: if the tag is not present in the table,
        // an empty entry is going to be inserted
        if((*curr_hash).size() < min_size) {
            min_hash = curr_hash;
            min_tag = tag;
            min_size = (*curr_hash).size();
        }
    }
    if(min_hash == NULL) {
        cout << "Empty tag!\n\n";
        return;
    }
    if((*min_hash).size() == 0) {
        cout << "There's an invalid tag in the query!\n\n";
        return;
    }
    bool intersection_empty = true;
    for(auto it = (*min_hash).begin(); it != (*min_hash).end(); ++it) {
        bool in_intersection = true;
        for(string tag : tags) {
            if(tag == min_tag) continue;
            if(!hash_tags.get(tag).has_key((*it).key)) {
                in_intersection = false;
                break;
            }
        }
        if(in_intersection) {
            intersection_empty = false;
            _print_player((*it).key, hash_players.get((*it).key));
        }
    }
    if(intersection_empty) {
        cout << "There's no intersection between the tags!\n";
    }
    cout << endl;
}

#endif // search_lib
