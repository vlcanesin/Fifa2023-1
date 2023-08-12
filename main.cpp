#include <iostream>
#include <vector>
#include <string>
#include "./includes/hash.hpp"

using namespace std;

const int M = 8000;

int main() {

    HashTable<HashTable<IntNode>> hash_of_hashes(M);

    string tag;
    int player_id;
    while(cin >> tag >> player_id && player_id != -1) {

        HashTable<IntNode> inp_node(M, tag);
        inp_node.content.id.value = player_id;   // PROVISÓRIO 

        IntNode int_node(player_id);
        inp_node.insert(int_node);

        hash_of_hashes.insert(inp_node);

    }

    for(vector<HashTable<IntNode>>& first_list : hash_of_hashes) {
        for(HashTable<IntNode>& first_node : first_list) {
            cout << "first node encontrado: " << first_node.id.value << endl;
            for(vector<IntNode> second_list : first_node) {
                for(IntNode second_node : second_list) {
                    cout << "(" << second_node.id.value << ") ";
                }
                if(second_list.size() > 0) cout << endl;
            }
        }
        if(first_list.size() > 0) cout << endl;
    }

    return 0;

}
