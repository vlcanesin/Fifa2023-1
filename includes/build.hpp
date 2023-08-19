#ifndef build_lib
#define build_lib

#include <iostream>
#include <vector>
#include <fstream>
#include "./trie.hpp"
#include "./hash.hpp"
#include "./heap.hpp"
#include "./csv.hpp"

using namespace std;

void build_tst_players(Tst &tst_players) {
    // Implementação - Guillermo
}

void build_hash_players(HashMap<int, Player> &hash_players) {

    io::CSVReader<2, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '"'>> doc_reviews("./../INF01124_FIFA21_clean/rating.csv");

    doc_reviews.read_header(io::ignore_extra_column, "sofifa_id", "rating");

    int id;
    float rating;
    while(doc_reviews.read_row(id, rating)){
        Player *player_pointer = &hash_players.get(id);
        player_pointer->n_reviews++;
        player_pointer->sum_reviews_x2 += (int)(2*rating);
    }

    io::CSVReader<3, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '"'>> doc_players("./../INF01124_FIFA21_clean/players.csv");

    doc_players.read_header(io::ignore_extra_column, "sofifa_id", "name", "player_positions");

    string name;
    string s_positions;
    while(doc_players.read_row(id, name, s_positions)){
        Player *player_pointer = &hash_players.get(id);
        (*player_pointer).name = name;
        s_positions += ", "; // delimiter
        string position_to_insert = "";
        for(int i = 0; i < s_positions.length()-1; i++) {
            if(s_positions[i] == ',' && s_positions[i+1] == ' ') {
                (*player_pointer).positions.push_back(position_to_insert);
                position_to_insert = "";
                i += 1; // skips ", "
            } else {
                position_to_insert += s_positions[i];
            }
        }
    }

}

void build_hash_users(HashMap<int, HeapMin> &hash_users) {
    // Implementação - Guillermo
}

void build_top_players(vector<Review> &top_players, HashMap<int, Player> &hash_players ) {

    io::CSVReader<3, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '"'>> doc_players("./../INF01124_FIFA21_clean/players.csv");
    doc_players.read_header(io::ignore_extra_column, "sofifa_id", "name", "player_positions");

    int id, i;
    string name, s_positions;

    HeapMin player_1000_review;
    Review player_reviews;

    while(doc_players.read_row(id,name,s_positions)){
        Player player = hash_players.get(id);
        if(player.n_reviews >= 1000){
            // Inserting negative values for higher evaluations being before in heap
            player_reviews.id = id;
            player_reviews.review = - (player.sum_reviews_x2 / (2 * float(player.n_reviews)));
            player_1000_review.push(player_reviews);
        }

    }
    // Copying ordered heap to vector
    top_players.resize(player_1000_review.size());
    for(i = 0; !player_1000_review.empty(); i++){
        player_reviews = player_1000_review.pop();
        top_players[i].id = player_reviews.id;
        top_players[i].review = -player_reviews.review;
    }

}

void build_hash_tags(HashMap<string, HashMap<int, int>> &hash_tags) {

    io::CSVReader<2, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '"'>> doc_tags("./../INF01124_FIFA21_clean/tags.csv");

    doc_tags.read_header(io::ignore_extra_column, "sofifa_id", "tag");

    int id;
    string tag;
    while(doc_tags.read_row(id, tag)){
        hash_tags.get(tag).get(id) = id;
    }

}

#endif // build_lib
