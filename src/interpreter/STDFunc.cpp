#include "STDFunc.h"
#include "Any.h"
#include <fstream>
#include <cstdlib>
streambuf* OutBuf = cout.rdbuf();
streambuf* InBuf = cin.rdbuf();

#define Def(x) Array x (unordered_map<string, Any>* ast, Array arr)
#define True Array(1, 1)
#define False Array(1, 0)

Def(Main) {
    return Array(1, 0);
}

Def(Print) {
    for (int i=0; i<arr.size(); i++)
        cout << arr[i];
    return arr;
}

Def(Println) {
    Print(ast, arr);
    cout << endl;
    return arr;
}

Def(Read) {
    for (int i=0; i<arr.size(); i++)
        cin >> arr[i];
    return arr;
}

Def(ArrayArgu) {
    return Array(1, arr[0][arr[1].Int()]);
}

Def(PushBack) {
    for (int i=1; i<arr.size(); i++)
        arr[0].push_back(arr[i]);
    return Array(1, arr[0]);
}

Def(PopBack) {
    arr[0].pop_back();
    return arr;
}

Def(Size) {
    return Array(1, Any(arr[0].size()));
}

#define DefDefVar(x, c) \
Def(x) {\
    AST *p = (AST*)arr[0].p;\
    Array tmp;\
    for (int i=0; i<p->son.size(); i++) {\
        Any(c).Copyto(*p->son[i].val);\
        tmp.push_back(*p->son[i].val);\
    }\
    return tmp;\
}

DefDefVar(DefVarInt, 0);
DefDefVar(DefVarDouble, 0.0);
DefDefVar(DefVarChar, (char)0);
DefDefVar(DefVarString, "");
DefDefVar(DefVarArray, Array());

#define DefDefOp(a, b) Def(a) {\
    Any sum;\
    arr[0].Copyto(sum);\
    for (int i=1; i<arr.size(); i++)\
        sum b arr[i];\
    return Array(1, sum);\
}

DefDefOp(Plus, +=);
DefDefOp(Minus, -=);
DefDefOp(Mul, *=);
DefDefOp(Div, /=);

#define DefDefOp2(a, b) Def(a) {\
    for (int i=1; i<arr.size(); i++)\
        arr[0] b arr[i];\
    return Array(1, arr[0]);\
}

DefDefOp2(PlusBy, +=);
DefDefOp2(MinusBy, -=);
DefDefOp2(MulBy, *=);
DefDefOp2(DivBy, /=);

Def(Copyto) {
    for (int i=0; i<arr.size()-1; i++)
        arr[arr.size()-1].Copyto(arr[i]);
    arr.pop_back();
    return arr;
}

Def(If) {
    AST *p = (AST*)arr[0].p;
    if (p->son[0].Exec()[0].Int()) {
        for (int i=1; i<p->son.size()-1; i++)
            p->son[i].Exec();
        return p->son[p->son.size()-1].Exec().Arr();
    }
    return Array(1, 0);
}

Def(While) {
    AST *p = (AST*)arr[0].p;
    while (p->son[0].Exec()[0].Int())
        for (int i=1; i<p->son.size(); i++)
            p->son[i].Exec();
    return Array(1, 0);
}

Def(For) {
    AST *p = (AST*)arr[0].p;
    for (p->son[0].Exec(); p->son[1].Exec()[0].Int(); p->son[2].Exec())
        for (int i=3; i<p->son.size(); i++)
            p->son[i].Exec();
    return Array(1, 0);
}

Def(ForEach) {
    AST *p = (AST*)arr[0].p;
    Any *i;
    i = new Any;
    *i = p->son[0].Exec()[0];
    for (*(int*)i->p = p->son[1].Exec()[0].Int(); i->Int() <= p->son[2].Exec()[0].Int(); ++ *(int*)i->p)
        for (int i=3; i<p->son.size(); i++)
            p->son[i].Exec();
    return Array(1, 0);
}

#define DefDefCompare(opname, op) \
Def(opname) {\
    int ans = 1;\
    for (int i=1; i<arr.size(); i++)\
        ans = (arr[i-1] op arr[i]) ? 1 : 0;\
    return Array(1, ans);\
}

DefDefCompare(EqualTo, ==);
DefDefCompare(NotEqualTo, !=);
DefDefCompare(LessThan, <);
DefDefCompare(MoreThan, >);
DefDefCompare(NoLessThan, >=);
DefDefCompare(NoMoreThan, <=);

Def(Not) {
    for (int i=1; i<arr.size(); i++)
        if (arr[i].Int())
            return False;
    return True;
}

Def(System) {
    return Array(1, (int)system(arr[0].Str().c_str()));
}

Def(Exit) {
    exit(0);
}

ifstream fin;
ofstream fout;
Def(ReadFrom) {
    string file = arr[0].Str();
    if (file == "/dev/stdin") {
        fin.close();
        cin.rdbuf(InBuf);
        return True;
    }
    fin.open(file);
    if (!fin)
        return False;
    cin.rdbuf(fin.rdbuf());
    return True;
}

Def(PrintTo) {
    string file = arr[0].Str();
    if (file=="/dev/stdout") {
        fout.close();
        cout.rdbuf(OutBuf);
        return True;
    }
    fout.open(file);
    if (!fout)
        return False;
    cout.rdbuf(fout.rdbuf());
    return True;
}

Def(EndOfFile) {
    return (cin.eof())?True:False;
}

void Init(AST& ast) {
    ast.preserved = false;

    ast.AddPreserved("int");
    ast.AddPreserved("double");
    ast.AddPreserved("string");
    ast.AddPreserved("char");
    ast.AddPreserved("array");

    ast.AddPreserved("if");
    ast.AddPreserved("while");
    ast.AddPreserved("for");
    ast.AddPreserved("foreach");

    ast.Add("main", Any(Main));
    ast.Add("if", Any(If));
    ast.Add("while", Any(While));
    ast.Add("for", Any(For));
    ast.Add("foreach", Any(ForEach));

    ast.Add("print", Any(Print));
    ast.Add("println", Any(Println));
    ast.Add("read", Any(Read));
    ast.Add("exit", Any(Exit));

    ast.Add("int", Any(DefVarInt));
    ast.Add("char", Any(DefVarChar));
    ast.Add("double", Any(DefVarDouble));
    ast.Add("string", Any(DefVarString));
    ast.Add("array", Any(DefVarArray));

    ast.Add("+", Any(Plus));
    ast.Add("-", Any(Minus));
    ast.Add("*", Any(Mul));
    ast.Add("/", Any(Div));
    ast.Add("=", Any(Copyto));
    ast.Add("+=", Any(PlusBy));
    ast.Add("-=", Any(MinusBy));
    ast.Add("*=", Any(MulBy));
    ast.Add("/=", Any(DivBy));
    ast.Add("[]", Any(ArrayArgu));

    ast.Add("==", Any(EqualTo));
    ast.Add("!=", Any(NotEqualTo));
    ast.Add("<=", Any(NoMoreThan));
    ast.Add(">=", Any(NoLessThan));
    ast.Add("<", Any(LessThan));
    ast.Add(">", Any(MoreThan));
    ast.Add("!", Any(Not));

    ast.Add("push", Any(PushBack));
    ast.Add("pop", Any(PopBack));
    ast.Add("size", Any(Size));

    ast.Add("system", Any(System));
    ast.Add("readfrom", Any(ReadFrom));
    ast.Add("printto", Any(PrintTo));
    ast.Add("eof", Any(EndOfFile));
}
