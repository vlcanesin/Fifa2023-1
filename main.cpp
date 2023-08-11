#include <iostream>
#include <vector>
#include <string>
#include "./includes/hash.hpp"
#include "./includes/csv-parser.hpp"

using namespace std;

const int M = 8000;

int main() {

    HashTable<PlayerNode> hash_players(M);

    PlayerNode inp_player;
    while(cin >> inp_player.id.value >> inp_player.name >> inp_player.n_reviews >> inp_player.sum_reviews_x2 && inp_player.id.value != -1) {
        hash_players.insert(inp_player);
    }

    cout << "Number of lists: " << hash_players.size() << endl;

    for(vector<PlayerNode> list : hash_players) {
        for(PlayerNode node : list) {
            cout << "(" << node.id.value << ", " << node.name << ", " << node.n_reviews << ", " << node.sum_reviews_x2 << ") ";
        }
        if(list.size() > 0) cout << endl;
    }

    return 0;

}
