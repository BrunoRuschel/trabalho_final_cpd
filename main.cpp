#include <iostream>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#define TAM 7993

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

int funcaoHash(int chave, vector<vector<JOGADOR>> &tb)
{
    return chave % int(tb.size());
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

void preenche_ratings(vector<vector<JOGADOR>> &tb, const string &nomeArquivo) {
    vector<JOGADOR> jogadores;
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

            // Read user_id
            getline(ss, token, ',');

            // Read sofifa_id
            getline(ss, token, ',');
            sofifa_id = stoi(token);

            // Read rating
            getline(ss, token, ',');
            nota = stof(token);

            int hashValue = funcaoHash(sofifa_id, tb);

            for (JOGADOR &jogador : tb[hashValue]) {
                if (jogador.id == sofifa_id) {
                    jogador.soma_ratings += nota;
                    jogador.count_ratings++;
                    break; // Once updated, exit the loop
                }
            }
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

int main()
{
    vector<vector<JOGADOR>> tb(TAM, vector<JOGADOR>());

    lerArquivo_fillTabela(tb, "players.csv");
    preenche_ratings(tb, "minirating.csv");

    JOGADOR jog;

    jog = busca(tb, 191043);

    cout << jog.soma_ratings / jog.count_ratings << endl;

    return 0;
}
