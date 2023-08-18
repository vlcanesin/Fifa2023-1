#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>

#include "./includes/trie.hpp"
#include "./includes/hash.hpp"
#include "./includes/heap.hpp"
#include "./includes/build.hpp"
#include "./includes/search.hpp"

using namespace std;

string strip(string s_to_strip);    // "python like" .strip() function

int main() {

    setlocale(LC_ALL, "en_US.UTF-8");

    // CONSTRUINDO AS ESTRUTURAS NECESSÁRIAS:
    // Colocar aqui todas as estruturas que devem ser geradas no início

    Tst tst_players;                                     // TST com o nome dos jogadores
    HashMap<int, Player> hash_players(10000);            // HashMap id_jogador -> jogadores 
    HashMap<int, HeapMin> hash_users;                    // HashMap id_usuário -> 20 melhores jogadores
    vector<Review> top_players;                          // vector ordenado de jogadores de +1000 reviews
    HashMap<string, HashMap<int, int>> hash_tags(1000);  // HashMap tag -> (HashMap id_jogador -> id_jogador)

    build_tst_players(tst_players);
    build_hash_players(hash_players);
    build_hash_users(hash_users);
    build_top_players(top_players);
    build_hash_tags(hash_tags);

    cout << "Build phase: done!\n";

    int cont = 0;
    cout << "First 10 tags with less than 20 elements:\n";
    for(auto it = hash_tags.begin(); it != hash_tags.end(); ++it) {
        if((*it).value.size() >= 20) continue;
        cout << "Tag: " << (*it).key << ", n_elements: " << (*it).value.size() << endl;
        for(auto innerit = (*it).value.begin(); innerit != (*it).value.end(); ++innerit) {
            Player curr_player = hash_players.get((*innerit).key);
            cout << "(key: " << (*it).key;
            cout << ", content: {" << curr_player.name
                           << ", " << curr_player.n_reviews 
                           << ", " << curr_player.sum_reviews_x2 << "})\n";
        }
        cout << endl;
        if(++cont >= 10) break;
    }

    // FAZENDO AS CONSULTAS NECESSÁRIAS:

    bool make_query = true;
    while(make_query) {

        string query_type;
        cin >> query_type;
        for(int i = 0; i < query_type.length(); i++) {
            query_type[i] = tolower(query_type[i]);
        }

        if(query_type == "player") {

            string player;
            getline(cin, player);

            search_player(strip(player), tst_players, hash_players);

        } else if(query_type == "user") {

            int user_id;
            cin >> user_id;

            search_user(user_id, hash_users, hash_players);

        } else if(query_type.substr(0, 3) == "top") {

            // Takes the last digits from "topNNN" as a number
            int n_players = stoi(query_type.substr(3, query_type.length()-3));

            string position;
            cin >> position;
            position = position.substr(1, position.length()-2);     // remove ''s

            search_top_n(n_players, position, top_players, hash_players);

        } else if(query_type == "tags") {
            
            vector<string> tags;
            string all_tags, tag;
            getline(cin, all_tags);
            istringstream iss(strip(all_tags));
            while(iss >> tag) {
                tags.push_back(tag.substr(1, tag.length()-2));
            }

            search_tags(tags, hash_tags, hash_players);

        } else if(query_type == "stop" || query_type == "exit" || query_type == "done") {
            make_query = false;
        } else {
            cout << "Invalid query!\n\n";
        }

    }

    return 0;

}

string strip(string s_to_strip) {

    size_t beg = s_to_strip.find_first_not_of(" \t\r\n");
    size_t end = s_to_strip.find_last_not_of(" \t\r\n");

    if(beg == string::npos || end == string::npos) {
        // string::npos indica "não encontrado"
        return "";   
    }
    
    return s_to_strip.substr(beg, beg-end + 1);
}