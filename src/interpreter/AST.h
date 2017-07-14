#ifndef MAZE_AST_H
#define MAZE_AST_H
#include "Any.h"
#include <vector>
#include <map>
#include <unordered_map>
class AST {
    public:
        vector<AST> son;
        Any* val;
        bool preserved = false;
        AST();
        AST(Any& x);
        AST(Function x);
        Any Exec();
        void Build(string name, istream& fin);
        void Clear();
        void Add(string name, Any x);
        void AddPreserved(string name);
        friend void Init(AST& ast);
};
#endif
