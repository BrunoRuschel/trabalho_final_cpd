#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>

#include "trieNode.h"

TrieNode* createNode(char value, TrieNode *left, TrieNode *middle, TrieNode *right, int id);
TrieNode* getNode(TrieNode *treeRoot, const std::string &key, int currentDigit);
TrieNode* putNode(TrieNode *treeRoot, const std::string &key, int currentDigit, int id);
void getAllWordsFromRoot(TrieNode* treeRoot, std::vector<int> &idList, bool includeFirst);
void getAllMatchingIDs(TrieNode* treeRoot, const std::string &key, std::vector<int> &idList);

#endif
