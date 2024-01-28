#include <iostream>
#include <vector>

using namespace std;

#ifndef SORTINGS_H_INCLUDED
#define SORTINGS_H_INCLUDED

void insertionSort_triple(std::vector<int>& id_jogador, std::vector<float>& nota_global, std::vector<float>& nota_user) {
    int n = id_jogador.size();

    for (int i = 1; i < n; ++i) {
        int current_id = id_jogador[i];
        float current_nota_global = nota_global[i];
        float current_nota_user = nota_user[i];

        int j = i - 1;

        // Ordena decrescentemente por nota_user e, em caso de empate, por nota_global
        while (j >= 0 && (nota_user[j] < current_nota_user || (nota_user[j] == current_nota_user && nota_global[j] < current_nota_global))) {
            id_jogador[j + 1] = id_jogador[j];
            nota_global[j + 1] = nota_global[j];
            nota_user[j + 1] = nota_user[j];
            --j;
        }

        id_jogador[j + 1] = current_id;
        nota_global[j + 1] = current_nota_global;
        nota_user[j + 1] = current_nota_user;
    }
}

void insertionSort_dec(vector<JOGADOR> &players) {
    int n = players.size();

    for (int i = 1; i < n; i++) {
        JOGADOR key = players[i];
        int j = i - 1;

        // Move elements of players[0..i-1] that are greater than key
        // to one position ahead of their current position
        float rating1 = players[j].count_ratings > 0 ? players[j].soma_ratings / players[j].count_ratings : 0;
        float rating2 = key.count_ratings > 0 ? key.soma_ratings / key.count_ratings : 0;
        while (j >= 0 && rating1 < rating2) {
            rating1 = players[j].count_ratings > 0 ? players[j].soma_ratings / players[j].count_ratings : 0;
            players[j + 1] = players[j];
            j = j - 1;
            rating1 = players[j].count_ratings > 0 ? players[j].soma_ratings / players[j].count_ratings : 0;
        }
        players[j + 1] = key;
    }
}

void insertionSort_cresc(vector<JOGADOR> &players) {
    int n = players.size();

    for (int i = 1; i < n; i++) {
        JOGADOR key = players[i];
        int j = i - 1;

        // Move elements of players[0..i-1] that are greater than key
        // to one position ahead of their current position
        while (j >= 0 && (players[j].soma_ratings / players[j].count_ratings) > (key.soma_ratings / key.count_ratings)) {
            players[j + 1] = players[j];
            j = j - 1;
        }
        players[j + 1] = key;
    }
}

#endif // SORTINGS_H_INCLUDED
