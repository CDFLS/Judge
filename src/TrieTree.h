#ifndef TRIETREE_H
#define TRIETREE_H
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <map>
using namespace std;
class TrieTree {
	private:
		int word;
		map<char,TrieTree> n;
	public:
		TrieTree() {
			word=-1;
		}
		TrieTree(vector<string> init) {
			for (int i=0;i<init.size();i++)
				this->add(init[i]);
		}
		void add(string str) {
			TrieTree* p=this;
			for (int i=0;i<str.length();i++) {
				p=&p->n[str[i]];
				if (p->word==-1)
					p->word=0;
			}
			p->word=1;
		}
		int query(string str,int l) {
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
		int query(string str) {
			return query(str,0);
		}
};
#endif
