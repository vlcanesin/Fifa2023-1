#ifndef search_lib
#define search_lib

#include <iostream>
#include <vector>
#include <iomanip>
#include <string.h>
#include "./trie.hpp"
#include "./hash.hpp"
#include "./heap.hpp"

const char SEP = ' ';
const int IDW = 10;
const int NAMEW = 46;
const int POSW = 18;
const int RATW = 10;
const int CNTW = 8;
const int USRW = 10;
const int GBRW = 16;

void setWidth(int width) {
    cout << right << setw(width) << setfill(SEP);
}

void _print_player_header() {
    setWidth(IDW); cout << "sofifa_id";
    setWidth(NAMEW); cout << "name";
    setWidth(POSW); cout << "player_positions";
    setWidth(RATW); cout << "rating";
    setWidth(CNTW); cout << "count" << endl;
}

void _print_player(int id, Player p) {
    setWidth(IDW); cout << id;
    setWidth(NAMEW); cout << p.name;
    string concat = "";
    for(string pos : p.positions) concat += pos + ", ";
    concat = concat.substr(0, concat.length()-2);
    setWidth(POSW); cout << concat;
    setWidth(RATW); cout << (float)p.sum_reviews_x2/(2*p.n_reviews);
    setWidth(CNTW); cout << p.n_reviews << endl;
}

void _print_user_header() {
    setWidth(IDW); cout << "sofifa_id";
    setWidth(NAMEW); cout << "name";
    setWidth(GBRW); cout << "global_rating";
    setWidth(CNTW); cout << "count";
    setWidth(USRW); cout << "rating" << endl;
}

void _print_user_review(Review user_review, Player p) {
    setWidth(IDW); cout << user_review.id;
    setWidth(NAMEW); cout << p.name;
    setWidth(GBRW); cout << (float)p.sum_reviews_x2/(2*p.n_reviews);
    setWidth(CNTW); cout << p.n_reviews;
    char review_3_digits[4];
    sprintf(review_3_digits, "%.1f", user_review.review);
    setWidth(USRW); cout << review_3_digits << endl;
}


void search_player(string player, Tst &tst_players, HashMap<int, Player> &hash_players) {
    vector <int>ids = tst_players.searchPrefix(player);
    if(ids.size() == 0) {
        cout << "Player not found!\n";
    } else {
        cout << endl;
        _print_player_header();
        for(auto& id_player: ids){
            Player player_with_prefix = hash_players.get(id_player);
            _print_player(id_player, player_with_prefix);
        }
    }
    cout << endl;
}

void search_user(int user_id, HashMap<int, HeapMin> &hash_users, HashMap<int, Player> hash_players) {
    HeapMin user_top20 = hash_users.get(user_id);
    Review found_review;
    vector<Review> review_top20;
    while(!user_top20.empty()){
        review_top20.insert(review_top20.begin(),user_top20.pop());
    }
    if(review_top20.size() == 0) {
        cout << "User not found!\n";
    } else {
        cout << endl;
        _print_user_header();
        for (auto player_review : review_top20){
            Player player = hash_players.get(player_review.id);
            _print_user_review(player_review, player);
        }
    }
    cout << endl;
}

void search_top_n(int n_players, string position, vector<Review> &top_players, HashMap<int, Player> &hash_players) {
    bool first = true;
    for(Review review : top_players) {
        if(n_players == 0) break;
        Player curr_player = hash_players.get(review.id);
        for(string curr_pos : curr_player.positions) {
            if(curr_pos == position) {
                if(first) {
                    cout << endl;
                    _print_player_header();
                    first = false;
                }
                _print_player(review.id, curr_player);
                n_players--;
                break;
            }
        }
    }
    cout << endl;
}

void search_tags(vector<string> &tags, HashMap<string, HashMap<int, int>> &hash_tags, HashMap<int, Player> &hash_players) {
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
    bool first = true;
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
            if(first) {
                cout << endl;
                _print_player_header();
                first = false;
            }
            _print_player((*it).key, hash_players.get((*it).key));
        }
    }
    if(intersection_empty) {
        cout << "There's no intersection between the tags!\n";
    }
    cout << endl;
}

#endif // search_lib
