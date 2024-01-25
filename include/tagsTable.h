#ifndef TAGSTABLE_H
#define TAGSTABLE_H

#include "string"
#include "vector"

using namespace std;

#define SIZE 3000

typedef struct Tag {
    int player_id;
    std::string text;
} Tag;

void insert_tag(vector<vector<int>> &tb_tags, const Tag& tag);
#endif
