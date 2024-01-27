#include "tagsTrie.h"

TagsTrieNode* createNodeTags(char value, TagsTrieNode *left, TagsTrieNode *middle, TagsTrieNode *right, int id) {
    auto* root = new TagsTrieNode;
    root->value = value;
    root->left = left;
    root->right = right;
    root->middle = middle;
    root->ids.push_back(id);
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
        nonNullableRoot->ids.push_back(id);
    }
    return nonNullableRoot;
};

void getAllMatchingIDsTags(TagsTrieNode* treeRoot, const string &key, vector<vector<int>> idList) {
    TagsTrieNode* firstNode = getNodeTags(treeRoot, key, 0);
    if(firstNode) {
        getAllWordsFromRootTags(treeRoot, std::move(idList));
    }
}

void getAllWordsFromRootTags(TagsTrieNode* treeRoot, vector<vector<int>> idList) {
    TagsTrieNode* currentIteration = nullptr;
    while(treeRoot && treeRoot->middle) {
        currentIteration = treeRoot->middle;
        if(!currentIteration->ids.empty()) {
            idList.push_back(currentIteration->ids);
        }
    }
}

