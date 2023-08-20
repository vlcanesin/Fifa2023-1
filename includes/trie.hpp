#ifndef tst_lib
#define tst_lib


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


class TstNode {
    public:
        TstNode *rightNode;
        TstNode *leftNode;
        TstNode *nextNode;
        char letter;
        vector<int> idPlayer;

        TstNode(char letterToInsert) : rightNode(nullptr), leftNode(nullptr), nextNode(nullptr), letter(letterToInsert), idPlayer() {}
};


class Tst{
    private:
        TstNode *root;

        void insertLetter(TstNode *&node, string name, int indexOfLetter, int idPlayerInsert){

            if(node == nullptr){    // Insert new letter
                node = new TstNode(name[indexOfLetter]);
                if(name.size() == indexOfLetter + 1)
                    node->idPlayer.push_back(idPlayerInsert);
                else
                    insertLetter(node->nextNode, name, indexOfLetter + 1, idPlayerInsert);
            }
            //Sideways Movement TST
            else if( node->letter < name[indexOfLetter])
                insertLetter(node->rightNode, name, indexOfLetter, idPlayerInsert);
            else if(node->letter > name[indexOfLetter])
                insertLetter(node->leftNode, name, indexOfLetter, idPlayerInsert);

            else{   // Search for next letter
                // Add player
                if(name.size() == indexOfLetter + 1)
                    node->idPlayer.push_back(idPlayerInsert);
                else
                    insertLetter(node->nextNode, name, indexOfLetter + 1, idPlayerInsert);
            }
        }

        TstNode* getLastLetterNode(TstNode *node, string name, int indexOfLetter, int sizeName){

            if(indexOfLetter + 1 == sizeName) return node;
            
            if(node->nextNode != nullptr && name[indexOfLetter] == node->letter)
                return getLastLetterNode(node->nextNode, name, indexOfLetter + 1, sizeName);

            else if(node->rightNode != nullptr && node->letter < name[indexOfLetter])
                return getLastLetterNode(node->rightNode, name, indexOfLetter, sizeName);

            else if (node->leftNode != nullptr && node->letter > name[indexOfLetter])
                return getLastLetterNode(node->leftNode, name, indexOfLetter, sizeName);

            return nullptr;
        }

        vector<int> getIdsPlayers(TstNode *node,string name){
            vector<int> idReturn;
            getIdsPlayersRecursion(node, name, &idReturn);
            return idReturn;
        }

        void getIdsPlayersRecursion(TstNode *node, string name, vector<int>* idReturn){
            if(node->leftNode != nullptr)
                getIdsPlayersRecursion(node->leftNode, name, idReturn);
            if(node->nextNode != nullptr)
                getIdsPlayersRecursion(node->nextNode, name + node->letter, idReturn);
            if(node->rightNode != nullptr)
                getIdsPlayersRecursion(node->rightNode, name, idReturn);


            if(!node->idPlayer.empty()){
                idReturn->insert(idReturn->end(), node->idPlayer.begin(), node->idPlayer.end());
            }
        }

    public:
        void insertName(string name, int idPlayer){
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            insertLetter(root,name, 0, idPlayer);
        }

        vector<int> searchPrefix(string name){
            TstNode* node;
            vector<int> idReturn(0);

            std::transform(name.begin(), name.end(), name.begin(), ::tolower);

            node = getLastLetterNode(root , name, 0, name.size());
            if(node == nullptr)
                return idReturn;

            else{
                if(node->nextNode != nullptr)
                    idReturn = getIdsPlayers(node->nextNode, name);

                if(!node->idPlayer.empty())
                    idReturn.insert(idReturn.end(), node->idPlayer.begin(), node->idPlayer.end());
            }

            return idReturn;
        }

        Tst(): root(nullptr) {}

};

#endif
