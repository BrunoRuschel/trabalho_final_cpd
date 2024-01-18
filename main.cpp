#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#define TAM 7993
#define TAM_TB_USER 10007 //19999979

using namespace std;

typedef struct {
    int id;
    string short_name;
    string long_name;
    string posicoes;
    string nacionalidade;
    string clube;
    string liga;
    float soma_ratings = 0.0;
    float count_ratings = 0.0;
}JOGADOR;

typedef struct {
    int id;
    vector<int> jogadores_avaliados;
    vector<float> notas;
}USER;

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


void lerArquivo_fillTabela(vector<vector<JOGADOR>> &tb, const string &nomeArquivo) {
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

int main()
{
    vector<vector<JOGADOR>> tb(TAM, vector<JOGADOR>());
    vector<vector<USER>> tb_user(TAM_TB_USER, vector<USER>());

    lerArquivo_fillTabela(tb, "players.csv");
    preenche_ratings(tb, "minirating.csv", tb_user);

    //JOGADOR jog;
    //USER user;

    //jog = busca(tb, 191043);
    //user = busca_user(tb_user, 21872);

    //cout << jog.soma_ratings / jog.count_ratings << endl;
    //cout << user.jogadores_avaliados[0] << " " << user.notas[0] << endl;
    //cout << user.jogadores_avaliados[1] << " " << user.notas[1] << endl;

    return 0;
}
