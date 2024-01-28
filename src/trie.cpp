#include "trie.h"

TrieNode* createNode(char value, TrieNode *left, TrieNode *middle, TrieNode *right, int id) {
    auto* root = (TrieNode*) malloc(sizeof(TrieNode));
    root->value = value;
    root->left = left;
    root->right = right;
    root->middle = middle;
    root->id = id;
    return root;
}

TrieNode* getNode(TrieNode *treeRoot, const std::string &key, int currentDigit) {
    char c = key[currentDigit];
    if (c < treeRoot->value) {
        return getNode(treeRoot->left, key, currentDigit);
    } else if (c > treeRoot->value) {
        return getNode(treeRoot->right, key, currentDigit);
    } else if (currentDigit < key.length() - 1) {
        return getNode(treeRoot->middle, key, ++currentDigit);
    }
    return treeRoot;
}

TrieNode* putNode(TrieNode *treeRoot, const std::string &key, int currentDigit, int id) {
    char c = key[currentDigit];
    TrieNode* nonNullableRoot = treeRoot;
    if (!treeRoot) {
        nonNullableRoot = createNode(c, nullptr, nullptr, nullptr, -1);
    }

    if(c < nonNullableRoot->value) {
        nonNullableRoot->left = putNode(nonNullableRoot->left, key, currentDigit, id);
    } else if (c > nonNullableRoot->value) {
        nonNullableRoot->right = putNode(nonNullableRoot->right, key, currentDigit, id);
    } else if (currentDigit < key.length() - 1) {
        nonNullableRoot->middle = putNode(nonNullableRoot->middle, key, ++currentDigit, id);
    } else {
        nonNullableRoot->id = id;
    }
    return nonNullableRoot;
};

void getAllMatchingIDs(TrieNode* treeRoot, const std::string &key, std::vector<int> &idList) {
    TrieNode* firstNode = getNode(treeRoot, key, 0);
    if(firstNode) {
        getAllWordsFromRoot(firstNode, idList, false);
    }
}

void getAllWordsFromRoot(TrieNode* treeRoot, std::vector<int> &idList, bool includeFirst) {
    TrieNode* currentIteration = nullptr;
    if(treeRoot) {
        currentIteration = treeRoot->middle;
        if(currentIteration && currentIteration->id > 0 && includeFirst) {
            idList.push_back(currentIteration->id);
        }
        getAllWordsFromRoot(currentIteration, idList, true);
        if(includeFirst) {
            getAllWordsFromRoot(treeRoot->left, idList, true);
            getAllWordsFromRoot(treeRoot->right, idList, true);
        }
    }
}

