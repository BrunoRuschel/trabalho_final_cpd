#include <iostream>
#include <vector>

using namespace std;

#ifndef SORTINGS_H_INCLUDED
#define SORTINGS_H_INCLUDED

void insertionSort(std::vector<int>& id_jogador, std::vector<float>& nota_global, std::vector<float>& nota_user) {
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

#endif // SORTINGS_H_INCLUDED
