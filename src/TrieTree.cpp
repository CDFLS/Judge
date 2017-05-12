#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "TrieTree.h"
using namespace std;

int ToUpper(int c) {
    return toupper(c);
}

TrieTree::TrieTree() {
    word=-1;
}

TrieTree::TrieTree(vector<string> init) {
    for (int i=0;i<init.size();i++)
        this->add(init[i]);
}

void TrieTree::add(string str) {
    transform(str.begin(), str.end(), str.begin(), ToUpper);
    TrieTree* p=this;
    for (int i=0;i<str.length();i++) {
        p=&p->n[str[i]];
        if (p->word==-1)
            p->word=0;
    }
    p->word=1;
}

int TrieTree::query(string str,int l) {
    transform(str.begin(), str.end(), str.begin(), ToUpper);
    TrieTree* p=this;
    for (int i=l;i<str.length();i++) {
        if (str[i]=='\\'||str[i]=='\n'||str[i]=='\r'||str[i]=='\t'||str[i]==' ')
            continue;
        p=&p->n[str[i]];
        if (p->word==-1)
            return -1;
        if (p->word==1)
            return i+1;
    }
    return -1;
}

int TrieTree::query(string str) {
    return query(str,0);
}
