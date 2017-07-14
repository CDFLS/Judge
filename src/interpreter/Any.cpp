#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include "Any.h"
#include "Message.h"
#include <sstream>
using namespace std;

string TransferToString(int k) {
    stringstream ss;
    ss << k;
    string ans;
    ss >> ans;
    return ans;
}

void Any::Delete() {
    if (*type == ANY_INT)
        delete (int*)p;
    else if (*type == ANY_DOUBLE)
        delete (double*)p;
    else if (*type == ANY_STRING)
        delete (string*)p;
    else if (*type == ANY_CHAR)
        delete (char*)p;
    else if (*type == ANY_ARRAY) {
        for (int i=0; i<((Array*)p)->size(); i++)
            (*(Array*)p)[i].Delete();
        delete (Array*)p;
    }
    else if (*type == ANY_FUNCTION)
        delete (Function*)p;
    delete type;
}

Any Any::Create(int t) {
    if (p != nullptr && t != *type)
        Delete();
    if (type == nullptr)
        type = new int;
    *type = t;
    if (*type == ANY_INT) {
        p = new int;
        *(int*)p = 0;
    }
    else if (*type == ANY_DOUBLE) {
        p = new double;
        *(double*)p = 0;
    }
    else if (*type == ANY_STRING) {
        p = new string;
        *(string*)p = "";
    }
    else if (*type == ANY_CHAR) {
        p = new char;
        *(char*)p = 0;
    }
    else if (*type == ANY_ARRAY)
        p = new Array;
    else if (*type == ANY_FUNCTION)
        p = new Function;
    return *this;
}

Any Any::operator += (Any& x) {
    if (*type != (*x.type) && (! ((*type == ANY_INT || *type == ANY_DOUBLE || *type == ANY_CHAR) && ((*x.type) == ANY_INT || (*x.type) == ANY_DOUBLE || (*x.type) == ANY_CHAR))))
        ErrorMsg("Invalid operation with different type of variables.");
    else if (*type == ANY_INT) {
        if ((*x.type) == ANY_INT)
            *(int*)p += *(int*)x.p;
        else if ((*x.type) == ANY_DOUBLE){
            int tmp = *(int*)p;
            this->Create(ANY_DOUBLE);
            *(double*)p = *(double*)x.p + tmp;
        } else
            *(int*)p += *(char*)x.p;
    } else if (*type == ANY_CHAR) {
        if ((*x.type) == ANY_INT)
            *(char*)p += *(int*)x.p;
        else if ((*x.type) == ANY_DOUBLE){
            char tmp = *(char*)p;
            this->Create(ANY_DOUBLE);
            *(double*)p = *(double*)x.p + tmp;
        } else
            *(char*)p += *(char*)x.p;
    } else if (*type == ANY_DOUBLE) {
        if ((*x.type) == ANY_INT)
            *(double*)p += *(int*)x.p;
        else
            *(double*)p += *(double*)x.p;
    } else if (*type == ANY_STRING)
        *(string*)p += *(string*)x.p;
    else if (*type == ANY_ARRAY) {
        for (int i=0; i<((Array*)x.p)->size(); i++)
            ((Array*)p)->push_back((*(Array*)x.p)[i]);
    } else
        ErrorMsg("Invalid operation with a function.");
    return *this;
}

#define DefOp(op, op2) Any Any::operator op (Any& x) {\
    if (*type != (*x.type) && (! ((*type == ANY_INT || *type == ANY_DOUBLE || *type == ANY_CHAR) && ((*x.type) == ANY_INT || (*x.type) == ANY_DOUBLE || (*x.type) == ANY_CHAR))))\
        ErrorMsg("Invalid operation with different type of variables.");\
    else if (*type == ANY_INT) {\
        if ((*x.type) == ANY_INT)\
            *(int*)p op *(int*)x.p;\
        else if ((*x.type) == ANY_DOUBLE){\
            int tmp = *(int*)p;\
            this->Create(ANY_DOUBLE);\
            *(double*)p = *(double*)x.p op2 tmp;\
        } else\
            *(int*)p op *(char*)x.p;\
    } else if (*type == ANY_CHAR) {\
        if ((*x.type) == ANY_INT)\
            *(char*)p op *(int*)x.p;\
        else if ((*x.type) == ANY_DOUBLE){\
            char tmp = *(char*)p;\
            this->Create(ANY_DOUBLE);\
            *(double*)p = *(double*)x.p op2 tmp;\
        } else\
            *(char*)p op *(char*)x.p;\
    } else if (*type == ANY_DOUBLE) {\
        if ((*x.type) == ANY_INT)\
            *(double*)p op *(int*)x.p;\
        else\
            *(double*)p op *(double*)x.p;\
    } else\
        ErrorMsg("Invalid operation.");\
    return *this;\
}

DefOp(-=, -);
DefOp(*=, +);
DefOp(/=, /);

Any Any::operator = (Function func) {
    *this = Any(func);
    return *this;
}

Any Any::operator = (Any x) {
    if (p != nullptr)
        this->Delete();
    type = new int;
    *type = (*x.type);
    p = x.p;
    return *this;
}

void Any::Copyto(Any& x) {
    if ((*x.type) != *type)
        x.Create(*type);
    if (*type == ANY_INT)
        *(int*)x.p = *(int*)p;
    else if (*type == ANY_DOUBLE)
        *(double*)x.p = *(double*)p;
    else if (*type == ANY_CHAR)
        *(char*)x.p = *(char*)p;
    else if (*type == ANY_STRING)
        *(string*)x.p = *(string*)p;
    else if (*type == ANY_ARRAY)
        *(Array*)x.p = *(Array*)p;
    else if (*type == ANY_FUNCTION)
        *(Function*)x.p = *(Function*)p;
}

Any::Any() {
    type = new int;
    *type = 0;
    p = nullptr;
}

Any::Any(int i) {
    type = new int;
    *type = ANY_INT;
    p = new int;
    *(int*)p = i;
}

Any::Any(double d) {
    type = new int;
    *type = ANY_DOUBLE;
    p = new double;
    *(double*)p = d;
}

Any::Any(char ch) {
    type = new int;
    *type = ANY_CHAR;
    p = new char;
    *(char*)p = ch;
}

Any::Any(string s) {
    type = new int;
    *type = ANY_STRING;
    p = new string;
    *(string*)p = s;
}

Any::Any(char* s) {
    type = new int;
    *type = ANY_STRING;
    p = new string;
    *(string*)p = (string)s;
}

Any::Any(const char* s) {
    type = new int;
    *type = ANY_STRING;
    p = new string;
    *(string*)p = (string)s;
}

Any::Any(vector<Any> arr) {
    type = new int;
    *type = ANY_ARRAY;
    p = new Array;
    *(Array*)p = arr;
}

Any::Any(Function func) {
    type = new int;
    *type = ANY_FUNCTION;
    p = new Function;
    *(Function*)p = func;
}

ostream& operator << (ostream& fout, Any x) {
    if ((*x.type) == ANY_INT)
        fout << *(int*)x.p;
    else if ((*x.type) == ANY_CHAR)
        fout << *(char*)x.p;
    else if ((*x.type) == ANY_DOUBLE)
        fout << *(double*)x.p;
    else if ((*x.type) == ANY_STRING)
        fout << *(string*)x.p;
    else if ((*x.type) == ANY_ARRAY) {
        fout << '[';
        for (int i=0; i<((Array*)x.p)->size()-1; i++)
            fout << (*(Array*)x.p)[i] << ", ";
        fout << (*(Array*)x.p)[((Array*)x.p)->size()-1] << ']';
    }
    else
        ErrorMsg("Cannot print: typeid = " + TransferToString(*x.type));
    return fout;
}

istream& operator >> (istream& fin, Any x) {
    if ((*x.type) == ANY_INT)
        fin >> *(int*)x.p;
    else if ((*x.type) == ANY_CHAR)
        fin >> *(char*)x.p;
    else if ((*x.type) == ANY_DOUBLE)
        fin >> *(double*)x.p;
    else if ((*x.type) == ANY_STRING) {
        *(string*)x.p = "";
        while (*(string*)x.p == "") {
            getline(fin, *(string*)x.p, '\n');
            while ((*(string*)x.p)[0] == ' ')
                ((string*)x.p)->erase(((string*)x.p)->begin());
        }
    }
    else if ((*x.type) == ANY_ARRAY)
        for (int i=0; i<((Array*)x.p)->size(); i++)
            fin >> (*(Array*)x.p)[i];
    else
        ErrorMsg("Cannot read: typeid = " + TransferToString(*x.type));
    return fin;
}

Any& Any::operator[] (int x) {
    if (*type == ANY_ARRAY) {
        return (*(Array*)p)[x];
    } else if (*type == ANY_STRING){
        Any* pt = new Any;
        *pt->type = ANY_CHAR;
        pt->p = &(*(string*)p)[x];
        return *pt;
    } else
        ErrorMsg("Not an array.");
}

void Any::push_back(Any x) {
    if (*type == ANY_ARRAY)
        ((Array*)p)->push_back(x);
    else
        ((string*)p)->push_back(x.Char());
}

void Any::pop_back() {
    if (*type == ANY_ARRAY)
        ((Array*)p)->pop_back();
    else
        ((string*)p)->pop_back();
}

int Any::size() {
    if (*type == ANY_ARRAY)
        return ((Array*)p)->size();
    else
        return ((string*)p)->size();
}

#define DefOp2(op1, op2) \
Any Any::operator op1 (Any& x) {\
    Any tmp;\
    tmp=*this;\
    tmp op2 x;\
    return tmp;\
}

DefOp2(+, +=);
DefOp2(-, -=);
DefOp2(*, *=);
DefOp2(/, /=);

int Any::Int() {
    if (*type != ANY_INT) {
        WarningMsg("Not an integer.");
        return 0;
    }
    return *(int*)p;
}

double Any::Double() {
    if (*type != ANY_DOUBLE) {
        WarningMsg("Not a double.");
        return 0;
    }
    return *(double*)p;
}

string Any::Str() {
    if (*type != ANY_STRING) {
        WarningMsg("Not a string.");
        return "";
    }
    return *(string*)p;
}

char Any::Char() {
    if (*type != ANY_CHAR) {
        WarningMsg("Not a char.");
        return 0;
    }
    return *(char*)p;
}

Array Any::Arr() {
    if (*type != ANY_ARRAY) {
        WarningMsg("Not an array.");
        return Array(0);
    }
    return *(Array*)p;
}

Function Any::Func() {
    if (*type != ANY_FUNCTION) {
        WarningMsg("Not a function.");
        return Function();
    }
    return *(Function*)p;
}

#define DefCompare(op) \
bool Any::operator op (Any x) const {\
    if (*type != *x.type)\
        return false;\
    if (*type == ANY_INT)\
        return *(int*)p op *(int*)x.p;\
    else if (*type == ANY_DOUBLE)\
        return *(double*)p op *(double*)x.p;\
    else if (*type == ANY_STRING)\
        return *(string*)p op *(string*)x.p;\
    else if (*type == ANY_CHAR)\
        return *(char*)p op *(char*)x.p;\
    else if (*type == ANY_ARRAY) {\
        bool ans = true;\
        if (((Array*)p)->size() != x.size())\
            return false;\
        for (int i=0; i<((Array*)p)->size(); i++)\
            if (!(((Array*)p)[i] op ((Array*)x.p)[i]))\
                return false;\
        return true;\
    }\
    else if (*type == ANY_FUNCTION)\
        ErrorMsg("Cannot compare between two functions.");\
    return false;\
}

DefCompare(==);
DefCompare(!=);
DefCompare(>=);
DefCompare(<=);
DefCompare(<);
DefCompare(>);
