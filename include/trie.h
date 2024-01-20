#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include "trieNode.h"

TrieNode createNode(char value, TrieNode *left, TrieNode *middle, TrieNode *right);
TrieNode* getNode(TrieNode *treeRoot, const std::string &key, int currentDigit);
void getAllWordsFromRoot(TrieNode* treeRoot, std::vector<int> idList);
void getAllMatchingIDs(TrieNode* treeRoot, const std::string &key, std::vector<int> idList);

#endif
