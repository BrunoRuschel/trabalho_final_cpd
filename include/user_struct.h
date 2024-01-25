#include <iostream>
#include <vector>

using namespace std;

#ifndef USER_STRUCT_H_INCLUDED
#define USER_STRUCT_H_INCLUDED

typedef struct {
    int id;
    vector<int> jogadores_avaliados;
    vector<float> notas;
}USER;

#endif // USER_STRUCT_H_INCLUDED
