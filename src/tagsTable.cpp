#include "tagsTable.h"

int hash_key(const std::string& key) {
    int value = 0;
    for (int i = 0; i < key.length(); i++) {
        value += int(key[i]);
    }
    return value % SIZE;
}

void insert_tag(vector<vector<int>> &tb_tags, const Tag& tag) {
    int hashedKey = hash_key(tag.text);

    for (int playerID : tb_tags[hashedKey]) {
        if (playerID == tag.player_id) {
            return;
        }
    }

    tb_tags[hashedKey].push_back(tag.player_id);
}
