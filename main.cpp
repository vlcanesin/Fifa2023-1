#include <iostream>
#include <vector>
#include <string>
#include "./includes/hash.hpp"
#include "./includes/csv-parser.hpp"

using namespace std;
using namespace rapidcsv;

const int M = 8000;

int main() {

    Document doc("data/INF01124_FIFA21_clean/players.csv");

    vector<int> ids = doc.GetColumn<int>("sofifa_id");
    vector<string> names = doc.GetColumn<string>("name");
    vector<string> positions = doc.GetColumn<string>("player_positions");

    vector<Player> players;

    for(int i = 0; i < ids.size(); i++) {
        Player input;
        input.id = ids[i];
        input.data.name = names[i];
        input.data.positions = positions[i];
        players.push_back(input);
    }

    HashTable table;
    InitializeHashTable(table, M);

    for(Player inputPlayer : players) {
        Insert(inputPlayer, table);
    }

    cout << "PARTE1: ESTATISTICAS DA TABELA HASH\n";

    HashStats hstats = CalculateHashStats(table);
    cout << "NUMERO DE ENTRADAS DA TABELA USADAS " << hstats.n_lists_used << endl;
    cout << "NUMERO DE ENTRADAS DA TABELA VAZIAS " << M-hstats.n_lists_used << endl;
    cout << "TAXA DE OCUPACAO " << (float)hstats.n_lists_used / M << endl;
    cout << "MINIMO TAMANHO DE LISTA " << hstats.min_size << endl;
    cout << "MAXIMO TAMANHO DE LISTA " << hstats.max_size << endl;
    cout << "MEDIO TAMANHO DE LISTA " << hstats.avg_size << "\n\n";

    cout << "PARTE2: ESTATISTICAS DAS CONSULTAS\n";

    int id, min_tests = MAX_ELEMENTS_PER_LIST, max_tests = 0;
    int acc = 0, n_searches = 0, acc_found = 0, n_searches_found = 0;

    while(!cin.eof() && cin >> id) {
        SearchStats sstats = SearchPlayer(id, table);
        cout << id << " " << sstats.name << " " << sstats.n_tests << endl;
        acc += sstats.n_tests;
        n_searches++;
        if(sstats.found) {
            min_tests = min(min_tests, sstats.n_tests);
            max_tests = max(max_tests, sstats.n_tests);
            acc_found += sstats.n_tests;
            n_searches_found++;
        }
    }

    cout << "MINIMO NUMERO DE TESTES POR NOME ENCONTRADO " << min_tests << endl;
    cout << "MAXIMO NUMERO DE TESTES POR NOME ENCONTRADO " << max_tests << endl;
    cout << "MEDIA NUMERO DE TESTES NOME ENCONTRADO " << (float)acc_found/n_searches_found << endl;
    cout << "MEDIA " << (float)acc/n_searches;

    return 0;

}
