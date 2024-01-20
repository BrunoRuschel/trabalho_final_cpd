#ifndef TRIENODE_H
#define TRIENODE_H

typedef struct TrieNode {
    TrieNode* left;
    TrieNode* middle;
    TrieNode* right;
    char value;
    int id; // Flag to indicate the end of a word
} TrieNode;

#endif
