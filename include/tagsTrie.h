#ifndef TAGSTRIE_H
#define TAGSTRIE_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct TagsTrieNode {
    TagsTrieNode* left = nullptr;
    TagsTrieNode* middle = nullptr;
    TagsTrieNode* right = nullptr;
    char value = (char) 0;
    vector<int> ids = vector<int>(); // Flag to indicate the end of a word
} TagsTrieNode;

TagsTrieNode* createNodeTags(char value, TagsTrieNode *left, TagsTrieNode *middle, TagsTrieNode *right, int id);
TagsTrieNode* getNodeTags(TagsTrieNode *treeRoot, const std::string &key, int currentDigit);
TagsTrieNode* putNodeTags(TagsTrieNode *treeRoot, const std::string &key, int currentDigit, int id);
void getAllMatchingIDsTags(TagsTrieNode* treeRoot, const string &key, vector<int>& idList);

#endif