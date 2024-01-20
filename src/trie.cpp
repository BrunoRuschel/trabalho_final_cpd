#include "trie.h"

TrieNode* createNode(TrieNode* root, char value, TrieNode *left, TrieNode *middle, TrieNode *right, int id) {
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
    if(!treeRoot) {
        TrieNode* newNode;
        treeRoot = createNode(newNode, c, nullptr, nullptr, nullptr, -1);
    }
    if(c < treeRoot->value) {
        treeRoot->left = putNode(treeRoot->left, key, currentDigit, id);
    } else if (c > treeRoot->value) {
        treeRoot->right = putNode(treeRoot->right, key, currentDigit, id);
    } else if (currentDigit < key.length() - 1) {
        treeRoot->middle = putNode(treeRoot->middle, key, ++currentDigit, id);
    } else {
        treeRoot->id = id;
    }
    return treeRoot;
};

void getAllMatchingIDs(TrieNode* treeRoot, const std::string &key, std::vector<int> idList) {
    TrieNode* firstNode = getNode(treeRoot, key, 0);
    if(firstNode) {
        getAllWordsFromRoot(treeRoot, std::move(idList));
    }
}

void getAllWordsFromRoot(TrieNode* treeRoot, std::vector<int> idList) {
    TrieNode* currentIteration = nullptr;
    while(treeRoot && treeRoot->middle) {
       currentIteration = treeRoot->middle;
        if(currentIteration->id > 0) {
            idList.push_back(currentIteration->id);
        }
    }
}

