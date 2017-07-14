#ifndef MAZE_ANY_H
#define MAZE_ANY_H
#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
using namespace std;
#define ANY_INT 1
#define ANY_DOUBLE 2
#define ANY_CHAR 3
#define ANY_STRING 4
#define ANY_ARRAY 5
#define ANY_FUNCTION 6
class Any {
    public:
        void *p;
        int* type;
        void Delete();
        Any Create(int type);
        Any operator += (Any& x);
        Any operator -= (Any& x);
        Any operator *= (Any& x);
        Any operator /= (Any& x);
        Any operator + (Any& x);
        Any operator - (Any& x);
        Any operator * (Any& x);
        Any operator / (Any& x);
        bool operator == (Any x) const;
        bool operator != (Any x) const;
        bool operator >= (Any x) const;
        bool operator <= (Any x) const;
        bool operator < (Any x) const;
        bool operator > (Any x) const;
        Any& operator [] (int x);
        Any operator = (Any x);
        Any operator = (function<vector<Any>(unordered_map<string, Any>* ast, vector<Any>)> func);
        void Copyto(Any& x);
        void push_back(Any x);
        void pop_back();
        int size();
        Any();
        Any(int i);
        Any(double d);
        Any(char ch);
        Any(string s);
        Any(char* s);
        Any(const char* s);
        Any(vector<Any> arr);
        Any(function<vector<Any>(unordered_map<string, Any>* ast, vector<Any>)> func);
        friend ostream& operator << (ostream& fout, Any x);
        friend istream& operator >> (istream& fin, Any x);
        int Int();
        double Double();
        string Str();
        char Char();
        vector<Any> Arr();
        function<vector<Any>(unordered_map<string, Any>* ast, vector<Any>)> Func();
};

typedef vector<Any> Array;
typedef function<Array(unordered_map<string, Any>* ast, Array)> Function;
#endif
