#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "tagsTable.h"
#include "trie.h"
#include "jogador_struct.h"
#include "user_struct.h"
#include "sortings.h"

#define TAM 7993
#define TAM_TB_USER  24999989 //10007
#define TAM_TB_TAGS 7993

using namespace std;

int funcaoHash(int chave, vector<vector<JOGADOR>> &tb)
{
    return chave % int(tb.size());
}

int funcaoHash_users(int chave, vector<vector<USER>> &tb_user)
{
    return chave % TAM_TB_USER;
}

void insere_user(vector<vector<USER>> &tb_user, USER &usuario)
{
    int id;
    bool achou = false;
    id = funcaoHash_users(usuario.id, tb_user);

    //se o user ja esta na tabela, apenas adiciona o jogador e a nota
    for (USER &user : tb_user[id]) {
        if (user.id == usuario.id) {
            user.jogadores_avaliados.insert(user.jogadores_avaliados.end(),
                                             usuario.jogadores_avaliados.begin(),
                                             usuario.jogadores_avaliados.end());
            user.notas.insert(user.notas.end(),
                                             usuario.notas.begin(),
                                             usuario.notas.end());
            achou = true;
        }
    }

    //se nao esta na tabela, adiciona esse usuario
    if(achou == false) {
        tb_user[id].push_back(usuario);
    }
}

void insere(vector<vector<JOGADOR>> &tb, JOGADOR jogador)
{
    int id;
    id = funcaoHash(jogador.id, tb);
    tb[id].push_back(jogador);
}

void loadTags(vector<vector<int>> &table, const string &filePath) {
    vector<Tag> tagArray;
    ifstream arquivo(filePath);

    if (arquivo.is_open()) {
        string linha;

        // Skip the header line
        getline(arquivo, linha);

        while (getline(arquivo, linha)) {
            stringstream ss(linha);
            string user_id;
            string player_id;
            string tag;

            Tag currentTag;

            // Skip user id (unused in queries)
            getline(ss, user_id, ',');

            getline(ss, player_id, ',');
            currentTag.player_id = stoi(player_id);

            // le nome longo
            getline(ss, tag, ',');
            currentTag.text = tag;

            insert_tag(table, currentTag);
        }

        arquivo.close();
    } else {
        cout << "Erro ao abrir o arquivo " << filePath << endl;
    }
}

void lerArquivo_fillTabela(vector<vector<JOGADOR>> &tb, TrieNode* stringTree, const string &nomeArquivo) {
    vector<JOGADOR> jogadores;
    ifstream arquivo(nomeArquivo);

    if (arquivo.is_open()) {
        string linha;

        // Skip the header line
        getline(arquivo, linha);

        // le cada linha do arquivo
        while (getline(arquivo, linha)) {
            stringstream ss(linha);
            string token;

            JOGADOR jogador;

            // le id
            getline(ss, token, ',');
            jogador.id = stoi(token);

            // le nome curto
            getline(ss, token, ',');
            jogador.short_name = token;

            // le nome longo
            getline(ss, token, ',');
            jogador.long_name = token;

            putNode(stringTree, token, 0, jogador.id);

            // le a posicao
            getline(ss, token, ',');
            if(token.front() == '"') {
                jogador.posicoes = token;
                getline(ss, token, ',');
                jogador.posicoes += token;
            }
            else {
                jogador.posicoes = token;
            }

            //nacionalidade
            getline(ss, token, ',');
            jogador.nacionalidade = token;

            //clube
            getline(ss, token, ',');
            jogador.clube = token;

            //liga
            getline(ss, token, ',');
            jogador.liga = token;

            //adiciona o jogador na tabela hash
            insere(tb, jogador);
        }

        arquivo.close();
    } else {
        cout << "Erro ao abrir o arquivo " << nomeArquivo << endl;
    }
}

void preenche_ratings(vector<vector<JOGADOR>> &tb, const string &nomeArquivo, vector<vector<USER>> &tb_user) {
    ifstream arquivo(nomeArquivo);
    int sofifa_id;
    float nota;

    if (arquivo.is_open()) {
        string linha;

        // Skip the header line
        getline(arquivo, linha);

        // Read each line from the file
        while (getline(arquivo, linha)) {
            stringstream ss(linha);
            string token;

            JOGADOR jogador;
            USER usuario;

            // Read user_id
            getline(ss, token, ',');
            usuario.id = stoi(token); //2.3

            // Read sofifa_id
            getline(ss, token, ',');
            sofifa_id = stoi(token);
            usuario.jogadores_avaliados.push_back(sofifa_id); //2.3

            // Read rating
            getline(ss, token, ',');
            nota = stof(token);
            usuario.notas.push_back(nota); //2.3

            int hashValue = funcaoHash(sofifa_id, tb);

            for (JOGADOR &jogador : tb[hashValue]) {
                if (jogador.id == sofifa_id) {
                    jogador.soma_ratings += nota;
                    jogador.count_ratings++;
                    break;
                }
            }
            insere_user(tb_user, usuario); //2.3
        }

        arquivo.close();
    } else {
        cout << "Erro ao abrir o arquivo " << nomeArquivo << endl;
    }
}

JOGADOR busca(vector<vector<JOGADOR>> &tb, int chave) {
    int id;
    JOGADOR jog_nulo;

    jog_nulo = {0, "null", "null","null","null","null","null", 0.0, 0.0};

    id = funcaoHash(chave, tb);

    for (const JOGADOR &jogador : tb[id]) {
        if (jogador.id == chave) {
            return jogador;
        }
    }

    return jog_nulo;
}

USER busca_user(vector<vector<USER>> &tb_user, int chave) {
    int id;
    USER user_nulo;

    user_nulo.id = 0;
    user_nulo.jogadores_avaliados.push_back(0);
    user_nulo.notas.push_back(0.0);

    id = funcaoHash_users(chave, tb_user);

    for (const USER &usuario : tb_user[id]) {
        if (usuario.id == chave) {
            return usuario;
        }
    }

    return user_nulo;
}

void jogadores_revisados(vector<vector<USER>> &tb_user, vector<vector<JOGADOR>> &tb, int id) {
    USER usuario;
    vector<JOGADOR> lista_jogadores;

    usuario = busca_user(tb_user, id);

    int tam = (int)usuario.jogadores_avaliados.size();
    vector<int> id_jogador(tam,0);
    vector<float> nota_user(tam,0.0);
    vector<float> nota_global(tam,0.0);

    //poe todos os jogadores revisados pelo usuario em uma lista
    for(int i = 0; i < tam; i++) {
            JOGADOR jog;

            jog = busca(tb, usuario.jogadores_avaliados[i]);
            id_jogador[i] = jog.id;
            nota_user[i] = usuario.notas[i];
            nota_global[i] = jog.soma_ratings / jog.count_ratings;
    }

    //ordena, considerando nota global e nota dada pelo usuario
    insertionSort_triple(id_jogador, nota_global, nota_user);

    cout << "id_jogador " << "short_name " << "long_name " << "nota_global count nota_user" << endl;

    for(int i = 0; i < 20; i++) {
        JOGADOR jog2 = busca(tb, id_jogador[i]);

        cout << id_jogador[i] << " " << jog2.short_name << " " << jog2.long_name << " " <<
         nota_global[i] << " " << jog2.count_ratings << " " << nota_user[i] << endl;
     }
}

void melhores_da_posicao(const vector<vector<JOGADOR>> &tb, string posicao, int n) {
    vector<JOGADOR> jogadores;
    string pos;
    bool achou = false;
    transform(posicao.begin(), posicao.end(), posicao.begin(), ::toupper);

    for (const vector<JOGADOR> &bucket : tb) {
        for (const JOGADOR &jog : bucket) {
            //checa se o jogador eh da posicao
            if ((jog.posicoes.find(posicao) != string::npos) && jog.count_ratings >= 1000) {
                jogadores.push_back(jog);
                achou = true;
            }
        }
    }

    //ordena
    insertionSort_dec(jogadores);

    if(achou){
        cout << "Melhores jogadores da posicao " << posicao << ":" << endl;
        for (int i = 0; i < n; i++) {
        cout << jogadores[i].id << " " << jogadores[i].short_name << " " << jogadores[i].long_name << " " << " " << jogadores[i].posicoes << " " << jogadores[i].nacionalidade << " " << jogadores[i].clube << " "
        << jogadores[i].liga << " " << 
         jogadores[i].soma_ratings / jogadores[i].count_ratings << " " << jogadores[i].count_ratings << endl;
        }
    }
    else {
        cout << "nenhum jogador encontrado com a posicao " << posicao << endl;
    }
}

void piores_da_posicao(const vector<vector<JOGADOR>> &tb, string posicao, int n) {
    vector<JOGADOR> jogadores;
    string pos;
    bool achou = false;
    transform(posicao.begin(), posicao.end(), posicao.begin(), ::toupper);

    for (const vector<JOGADOR> &bucket : tb) {
        for (const JOGADOR &jog : bucket) {
            //checa se o jogador eh da posicao
            if ((jog.posicoes.find(posicao) != string::npos) && jog.count_ratings >= 1000) {
                jogadores.push_back(jog);
                achou = true;
            }
        }
    }

    //ordena
    insertionSort_cresc(jogadores);

    if(achou){
        cout << "Piores jogadores da posicao " << posicao << ":" << endl;
        for (int i = 0; i < n; i++) {
        cout << jogadores[i].id << " " << jogadores[i].short_name << " " << jogadores[i].long_name << " " << " " << jogadores[i].posicoes << " " << jogadores[i].nacionalidade << " " << jogadores[i].clube << " "
        << jogadores[i].liga << " " << 
         jogadores[i].soma_ratings / jogadores[i].count_ratings << " " << jogadores[i].count_ratings << endl;
        }
    }
    else {
        cout << "nenhum jogador encontrado com a posicao " << posicao << endl;
    }
}

int main()
{
    vector<vector<JOGADOR>> tb(TAM, vector<JOGADOR>());
    vector<vector<USER>> tb_user(TAM_TB_USER, vector<USER>());
    vector<vector<int>> tb_tags(TAM_TB_TAGS, vector<int>());
    TrieNode* tst = createNode(' ', nullptr, nullptr, nullptr, -1);


    lerArquivo_fillTabela(tb, tst, "../data-files/players.csv");
    preenche_ratings(tb, "../data-files/minirating.csv", tb_user);
    loadTags(tb_tags, "../data-files/tags.csv");

    //JOGADOR jog;
    //USER user;

    //jog = busca(tb, 191043);
    //user = busca_user(tb_user, 21872);

    //cout << jog.soma_ratings / jog.count_ratings << endl;
    //cout << user.jogadores_avaliados[0] << " " << user.notas[0] << endl;
    //cout << user.jogadores_avaliados[1] << " " << user.notas[1] << endl;

    return 0;
}
