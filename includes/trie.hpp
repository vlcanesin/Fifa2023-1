#ifndef tst_lib
#define tst_lib
#endif


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Forward declarations

class Tst;
class TstNode;

// CLASSES USED FOR TST (PLAYER NAMES)


class TstNode {
    public:
        TstNode *rightNode;
        TstNode *leftNode;
        TstNode *nextNode;
        char letter;
        int idPlayer;

        TstNode(char letterToInsert) : rightNode(nullptr), leftNode(nullptr), nextNode(nullptr), letter(letterToInsert), idPlayer(0) {
        }
};


class Tst{
    private:
        TstNode *root;

        void insertLetter(TstNode **node, string name, int indexOfLetter, int idPlayer){
            if(*node == nullptr){    // Se o nodo ainda nao foi criado, o caractere deve ser inserido no novo nodo
                *node = new TstNode(name[indexOfLetter]);
                if(name.size() == indexOfLetter + 1) // Se o caractere for de fim de string, pode-se finalizar a insercao e salva-se o id
                    (*node)->idPlayer = idPlayer;

                else
                    insertLetter(&(*node)->nextNode, name, indexOfLetter + 1, idPlayer);
            }

            else if( (*node)->letter < name[indexOfLetter]){ // Se o caractere que vai ser inserido for maior que o do nodo, deve-se olhar o caractere da direita
                insertLetter(&(*node)->rightNode, name, indexOfLetter, idPlayer);
            }

            else if((*node)->letter > name[indexOfLetter]){    // Se o caractere que vai ser inserido for menor que o do nodo, deve-se olhar o caractere da direita
                insertLetter(&(*node)->leftNode, name, indexOfLetter, idPlayer);
            }

            else{   // Se for igual, segue-se para o pr�ximo caractere sem precisar inserir
                if(name.size() == indexOfLetter + 1){ // Se alguem tiver um nome contido, insere-se da mesma maneira
                    (*node)->idPlayer = idPlayer;
                }
                else
                    insertLetter(&(*node)->nextNode, name, indexOfLetter + 1, idPlayer);
            }
        }

        TstNode* getNodeName(string name){
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            return  getLastLetterNode(root , name, 0, name.size());
        }

        vector<int> getIdsPlayers(TstNode *node,string name){

            vector<int> idReturn;

            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            getIdsPlayersRecursion(node, name, &idReturn);

            return idReturn;
        }

        TstNode* getLastLetterNode(TstNode *node, string name, int indexOfLetter, int sizeName){

            if(name[indexOfLetter] == node->letter && indexOfLetter + 1 != sizeName)
                node = getLastLetterNode(node->nextNode, name, indexOfLetter + 1, sizeName);

            else if(node->letter < name[indexOfLetter] && node->rightNode != nullptr)
                node = getLastLetterNode(node->rightNode, name, indexOfLetter, sizeName);

            else if (node->leftNode != nullptr)
               node = getLastLetterNode(node->leftNode, name, indexOfLetter, sizeName);

            else if(indexOfLetter + 1 != sizeName) // Se a string nao tiver acabado mas nao houver nenhum nodo, nada foi encontrado
                node = nullptr;

            return node;
        }

        void getIdsPlayersRecursion(TstNode *node, string name, vector<int>* idReturn){
            if(node->rightNode != nullptr)
                getIdsPlayersRecursion(node->rightNode, name, idReturn);
            if(node->leftNode != nullptr)
                getIdsPlayersRecursion(node->leftNode, name, idReturn);
            if(node->nextNode != nullptr)
                getIdsPlayersRecursion(node->nextNode, name + node->letter, idReturn);
            if(node->idPlayer != 0)
                idReturn->push_back(node->idPlayer);
        }

    public:
        void insertName(string name, int idPlayer){
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            insertLetter(&root,name, 0, idPlayer);
        }

        vector<int> searchPrefix(string name){

            TstNode * node = getNodeName(name);
            vector<int> idReturn;
            if(node != nullptr){
                idReturn = getIdsPlayers(node->nextNode, name);
                if(node->idPlayer != 0)
                    idReturn.push_back(node->idPlayer);
            }
            return idReturn;
        }

        Tst(): root(nullptr) {}

};

