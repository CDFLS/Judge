#include "AST.h"
#include <map>
#include <unordered_map>
#include <sstream>
#include "Message.h"
unordered_map<string, Any> Var;
map<string, bool> Preserved;

template<typename T>
T Transfer(string str) {
    stringstream ss;
    ss << str;
    T ans;
    ss >> ans;
    return ans;
}

string Transfer(int integer) {
    stringstream ss;
    ss << integer;
    string ans;
    ss >> ans;
    return ans;
}

void AST::AddPreserved(string name) {
    Preserved[name] = true;
}

AST::AST() {
    Init(*this);
    preserved = false;
}

AST::AST(Any& x) {
    val=&x;
    preserved = false;
}

AST::AST(Function x) {
    *val=Any(x);
    preserved = false;
}

Any AST::Exec() {
    if (*val->type != ANY_FUNCTION)
        return Array(1, *val);
    Array para;
    if (this->preserved) {
        Any tmp;
        tmp.p = this;
        para.push_back(tmp);
    } else for (int i=0; i<son.size(); i++) {
        Any tmp = son[i].Exec();
        if (*tmp.type == 0)
            ErrorMsg("Undefined variable.");
        for (int i=0; i<tmp.size(); i++)
            para.push_back(tmp[i]);
    }
    if (*val->type != ANY_FUNCTION)
        ErrorMsg("Not a function.");
    Array tmp = val->Func()(&Var, para);
    return tmp;
}

void AST::Add(string name, Any x) {
    Var[name]=x;
}

void AST::Build(string name, istream& fin) {
    string funcname=name;
    val = &Var[name];
    bool NotEnd=true;
    while (NotEnd) {
        name = "";
        char ch = 0;
        int readingstr = -1;
        while (ch != ' ' || (ch == ' ' && readingstr == 1)) {
            fin.read(&ch, 1);
            if (!fin)
                ErrorMsg("Program unfinished.");
            if (ch == '\n' || ch == '\r' || ch == '\t')
                ch = ' ';
            if (ch == '"' || ch =='\'')
                readingstr *= -1;
            if (readingstr == 1) {
                if (ch == '\\') {
                    fin.read(&ch, 1);
                    if (ch == 'n')
                        ch = '\n';
                    if (ch == 'r')
                        ch = '\r';
                    if (ch == 't')
                        ch = '\t';
                }
                name.push_back(ch);
            } else {
                if (ch == ';') {
                    string tmp;
                    getline(fin, tmp, '\n');
                    break;
                }
                name.push_back(ch);
            }
            if (ch == ')')
                break;
        }
        while (name[name.length()-1] == ' ')
            name.pop_back();
        if (name[name.length()-1] == ')') {
            NotEnd = false;
            name.pop_back();
        }
        if (name == "")
            continue;
        //cout << funcname << ": " << name << endl;
        if (name[0] == '(') {
            string tmp = name.substr(1, name.length()-1);
            son.push_back(AST(Var[tmp]));
            if (Preserved[tmp])
                son[son.size()-1].preserved = true;
            son[son.size()-1].Build(tmp, fin);
        } else if (name[0] == '"' && name[name.length()-1] == '"') {
            Any *p = new Any;
            *p=name.substr(1, name.length()-2);
            son.push_back(AST(*p));
        } else if (name[0] == '\'' && name[name.length()-1] == '\'') {
            Any *p = new Any;
            *p=name.substr(1, name.length()-2)[0];
            son.push_back(AST(*p));
        } else {
            bool is_int = true;
            int dotnum = 0;
            for (int i=0; i<name.length(); i++)
                if (!isdigit(name[i])) {
                    if (name[i]=='.')
                        dotnum++;
                    else
                        is_int = false;
                }
            if (is_int && dotnum == 0) {
                Any *p = new Any;
                *p = Transfer<int>(name);
                son.push_back(AST(*p));
            } else if (dotnum == 1) {
                Any *p = new Any;
                *p = Transfer<double>(name);
                son.push_back(AST(*p));
            } else {
                Var[name]=Any();
                son.push_back(Var[name]);
            }
        }
    }
}

void AST::Clear() {
    if (val->type != nullptr)
        val->Delete();
    val = nullptr;
    while (val == nullptr)
    for (int i=0; i<son.size(); i++)
        son[i].Clear();
    for (int i=0; i<son.size(); i++)
        son.pop_back();
}
