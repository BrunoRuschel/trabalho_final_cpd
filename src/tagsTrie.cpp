#include "tagsTrie.h"

TagsTrieNode* createNodeTags(char value, TagsTrieNode *left, TagsTrieNode *middle, TagsTrieNode *right, int id) {
    auto* root = new TagsTrieNode;
    root->value = value;
    root->left = left;
    root->right = right;
    root->middle = middle;
    if(id > 0) {
        root->ids.push_back(id);
    }
    return root;
}

TagsTrieNode* getNodeTags(TagsTrieNode *treeRoot, const std::string &key, int currentDigit) {
    char c = key[currentDigit];
    if (c < treeRoot->value) {
        return getNodeTags(treeRoot->left, key, currentDigit);
    } else if (c > treeRoot->value) {
        return getNodeTags(treeRoot->right, key, currentDigit);
    } else if (currentDigit < key.length() - 1) {
        return getNodeTags(treeRoot->middle, key, ++currentDigit);
    }
    return treeRoot;
}

TagsTrieNode* putNodeTags(TagsTrieNode *treeRoot, const std::string &key, int currentDigit, int id) {
    char c = key[currentDigit];
    TagsTrieNode* nonNullableRoot = treeRoot;
    if (!treeRoot) {
        nonNullableRoot = createNodeTags(c, nullptr, nullptr, nullptr, -1);
    }

    if(c < nonNullableRoot->value) {
        nonNullableRoot->left = putNodeTags(nonNullableRoot->left, key, currentDigit, id);
    } else if (c > nonNullableRoot->value) {
        nonNullableRoot->right = putNodeTags(nonNullableRoot->right, key, currentDigit, id);
    } else if (currentDigit < key.length() - 1) {
        nonNullableRoot->middle = putNodeTags(nonNullableRoot->middle, key, ++currentDigit, id);
    } else {
        auto ids = nonNullableRoot->ids;
        if(find(ids.begin(), ids.end(), id) != ids.end()){
            return nonNullableRoot;
        }
        if(nonNullableRoot->ids.size() == 1 && nonNullableRoot->ids[0] == -1) {
            nonNullableRoot->ids[0] = id;
        } else {
            nonNullableRoot->ids.push_back(id);
        }

    }
    return nonNullableRoot;
};

void getAllMatchingIDsTags(TagsTrieNode* treeRoot, const string &key, vector<int>& idList) {
    TagsTrieNode* firstNode = getNodeTags(treeRoot, key, 0);
    if(firstNode) {
        for(int id : firstNode->ids){
            idList.push_back(id);
        }
    }
}

