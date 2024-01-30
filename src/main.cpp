#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "trie.h"
#include "jogador_struct.h"
#include "user_struct.h"
#include "sortings.h"
#include "tagsTrie.h"
#include "printer.h"

#define TAM 7993
#define TAM_TB_USER  24999989 //10007

using namespace std;

TagsTrieNode * loadTags(const string &filePath) {
    ifstream arquivo(filePath);

    auto* tst = new TagsTrieNode;

    if (arquivo.is_open()) {
        string linha;

        // Skip the header line
        getline(arquivo, linha);

        while (getline(arquivo, linha)) {
            stringstream ss(linha);
            string user_id;
            string player_id;
            string tag;

            // Skip user id (unused in queries)
            getline(ss, user_id, ',');

            getline(ss, player_id, ',');
            getline(ss, tag, ',');

            if(!tag.empty() && !player_id.empty()) {
                //Uppercase tag
                transform(tag.begin(), tag.end(), tag.begin(), ::toupper);
                putNodeTags(tst, tag, 0, stoi(player_id));
            }
        }

        arquivo.close();
        return tst;
    } else {
        cout << "Erro ao abrir o arquivo " << filePath << endl;
        return nullptr;
    }
}

void handlePlayerQuery(TrieNode* treeRoot, vector<vector<JOGADOR>> &table, const string& query) {
    vector<int> idList;
    vector<JOGADOR> playerList;
    if(!query.empty()) {
        getAllMatchingIDs(treeRoot, query, idList);
    }
    //Com a lista de IDS, buscar jogadores em outras estruturas.
    for(int id : idList) {
        JOGADOR player = busca(table, id);
        if(player.id == id) {
            playerList.push_back(player);
        }
    }
    insertionSort_dec(playerList);
    printPlayerInfo(playerList);
}

void handleTagQuery(const vector<string>& tags, TagsTrieNode* tags_tst, vector<vector<JOGADOR>> &table) {
    vector<int> idList;
    vector<int> tempIdList;
    bool firstIteration = true;
    for(string tag : tags) {
        tag.erase(std::remove(tag.begin(), tag.end(), '\''), tag.end());
        transform(tag.begin(), tag.end(), tag.begin(), ::toupper);

        getAllMatchingIDsTags(tags_tst, tag, tempIdList);
        if(firstIteration) {
            firstIteration = false;
            idList.assign(tempIdList.begin(), tempIdList.end());
            tempIdList.clear();
        } else {
            if(tempIdList.empty() || idList.empty()) {
                idList.clear();
                tempIdList.clear();
                continue;
            }
            //Intersection of tags (could be improved with a search algo)
            for (int i = 0; i < idList.size(); i++) {
                bool found = false;
                for (int j : tempIdList) {
                    if(idList[i] == j) {
                        found = true;
                    }
                }
                if(!found){
                    idList.erase(idList.begin()+i);
                    i--;
                }
            }
            tempIdList.clear();
        }
    }
    vector<JOGADOR> playerList;
    for(int id : idList) {
        JOGADOR player = busca(table, id);
        if(player.id == id) {
            playerList.push_back(player);
        }
    }
    insertionSort_dec(playerList);
    printPlayerInfo(playerList);
}

int main()
{
    vector<vector<JOGADOR>> tb(TAM, vector<JOGADOR>());
    vector<vector<USER>> tb_user(TAM_TB_USER, vector<USER>());
    createNodeTags(' ', nullptr, nullptr, nullptr, -1);
    TrieNode* players_tst = createNode(' ', nullptr, nullptr, nullptr, -1);


    lerArquivo_fillTabela(tb, players_tst, "../data-files/players.csv");
    preenche_ratings(tb, "../data-files/minirating.csv", tb_user);
    TagsTrieNode* tags_tst = loadTags("../data-files/tags.csv");

    cout << "Aperte CTRL + C para interromper a execução" << endl;

    string command;
    while (getline(std::cin, command) && !command.empty()) {
        istringstream iss(command);
        vector<string> words;

        // Read each word from the stringstream
        string word;
        string tempWord = "";
        while (iss >> word) {
            if(word[0] == '\'' && word.back() != '\'') {
                tempWord = word;
            } else if (word[0] != '\'' && word.back() == '\'') {
                tempWord.erase(tempWord.begin());
                word.pop_back();

                word = tempWord + " " + word;
                tempWord = "";
                words.push_back(word);
                continue;
            } else if (word[0] != '\'' && word.back() != '\'' && !tempWord.empty()) {
                tempWord += " " + word;
            } else {
                if(word[0] == '\'' && word.back() == '\'') {
                    word.erase(word.begin());
                    word.pop_back();
                }
                tempWord = "";
                words.push_back(word);
            }
        }

        if(!words.empty()) {
            if(words[0] == "player") {
                auto query = words[1];
                transform(query.begin(), query.end(), query.begin(), ::toupper);
                handlePlayerQuery(players_tst, tb, query);
            }

            if(words[0] == "tags") {
                words.erase(words.begin());
                handleTagQuery(words, tags_tst, tb);
            }

            if (words[0] == "user" && words.size() == 2) {
                int userId = stoi(words[1]);
                jogadores_revisados(tb_user, tb, userId);
            }

            if (words[0] == "top" && words.size() == 3) {
                int n = stoi(words[1]);
                string posicao = words[2];
                melhores_da_posicao(tb, posicao, n);
            }

            if (words[0] == "piores" && words.size() == 3) {
                int n = stoi(words[1]);
                string posicao = words[2];
                piores_da_posicao(tb, posicao, n);
            }
        }
    }

    return 0;
}
