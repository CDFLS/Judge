#ifndef TRIETREE_H
#define TRIETREE_H
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
class TrieTree {
    private:
        int word;
        map<char,TrieTree> n;
    public:
        TrieTree();
        TrieTree(vector<string> init);
        void add(string str);
        int query(string str,int l);
        int query(string str);
};
#endif
