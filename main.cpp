#include <iostream>
#include <vector>
#include <string>
#include "./includes/hash.hpp"

using namespace std;

const int M = 3;

int main() {

    HashMap<int, Player> player_hash(M);
    if(player_hash.empty()) cout << "The hash is initially empty\n";

    int k;
    Player p;
    while(cin >> k >> p.name >> p.n_reviews >> p.sum_reviews_x2 && k != -1) {
        HashNode<int, Player> node_to_insert(k, p);
        player_hash.insert(node_to_insert);
    }

    if(!player_hash.empty()) cout << "The hash is not empty\n";

    for(HashMap<int, Player>::Iterator it = player_hash.begin(); it != player_hash.end(); ++it) {
        cout << "(key: " << (*it).key;
        cout << ", content: {" << (*it).value.name << ", " << (*it).value.n_reviews << ", " << (*it).value.sum_reviews_x2 << "})\n";
    }

    cout << "Table successfully read!\n";

    return 0;

}
