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

void build_hash_player_users(HashMap<int, Player> &hash_players, HashMap<int, HeapMin> &hash_users){

    io::CSVReader<3, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '"'>> doc_reviews("./../INF01124_FIFA21_clean/rating.csv");

    doc_reviews.read_header(io::ignore_extra_column, "user_id","sofifa_id", "rating");

    int player_id, user_id;
    float rating;
    while(doc_reviews.read_row(user_id, player_id, rating)){
        // Hash Players build
        Player *player_pointer = &hash_players.get(player_id);
        player_pointer->n_reviews++;
        player_pointer->sum_reviews_x2 += (int)(2*rating);

        // Hash USers Build
        HeapMin *heap_user_pointer = &(hash_users.get(user_id)).limit_size(20);
        Review user_review;

        user_review.id = player_id;
        user_review.review = rating;
        (*heap_user_pointer).push(user_review);
    }

    io::CSVReader<3, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '"'>> doc_players("./../INF01124_FIFA21_clean/players.csv");

    doc_players.read_header(io::ignore_extra_column, "sofifa_id", "name", "player_positions");

    string name;
    string s_positions;
    while(doc_players.read_row(player_id, name, s_positions)){
        Player *player_pointer = &hash_players.get(player_id);
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

void build_tst_players(Tst &tst_players) {
    io::CSVReader<2, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '"'>> doc_players("./../INF01124_FIFA21_clean/players.csv");

    doc_players.read_header(io::ignore_extra_column, "sofifa_id", "name");
    int id;
    string name;

      while(doc_players.read_row(id, name)){
        tst_players.insertName(name, id);
    }
}

void build_top_players(vector<Review> &top_players, HashMap<int, Player> &hash_players ) {

    HeapMin player_1000_review;
    int i;
    Review insert_review;

    for(auto player_review = hash_players.begin(); player_review != hash_players.end(); ++player_review){
        if((*player_review).value.n_reviews >= 1000){
             //Inserting negative values for higher evaluations being before in heap
            insert_review.id  =(*player_review).key;
            insert_review.review = - ((*player_review).value.sum_reviews_x2 / (2 * float((*player_review).value.n_reviews)));
            player_1000_review.push(insert_review);
        }
    }

    // Copying ordered heap to vector
    top_players.resize(player_1000_review.size());
    for(i = 0; !player_1000_review.empty(); i++){
        insert_review = player_1000_review.pop();
        top_players[i].id = insert_review.id;
        top_players[i].review = -insert_review.review;
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
