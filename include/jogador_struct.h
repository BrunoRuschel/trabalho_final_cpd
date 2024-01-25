#include <iostream>
#include <vector>
#include <string>

using namespace std;

#ifndef JOGADOR_STRUCT_H_INCLUDED
#define JOGADOR_STRUCT_H_INCLUDED

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

#endif // JOGADOR_STRUCT_H_INCLUDED
