#ifndef MULTICOMPILER_H
#define MULTICOMPILER_H
#include "judge.h"
#include <iostream>
#include <string>
using namespace std;
struct COMPILER {
    string suffix, dependency, cmd;
    int usable;
};
static int Inited=0;
static vector<COMPILER> Compiler={
    {"cpp","g++ -dumpversion","timeout 10s g++ -static -std=c++11 -s -lm -DEJUDGE %s -o Exec/%s",0},
    {"c","gcc -dumpversion","timeout 10s gcc -static -std=c99 -fno-asm -s -lm -DEJUDGE %s -o Exec/%s",0},
    {"pas","fpc -iV","timeout 10s fpc -Xs -Sgic -dEJUDGE %s -oExec/%s",0},
    {"py","python3 --version","judge python %s Exec/%s",0},
    {"py","python2 --version","judge python2 %s Exec/%s",0},
};
static void InitCompiler(){
    Inited=1;
    for (int i=0;i<Compiler.size();i++) {
        int tmp;
        if (WEXITSTATUS(tmp=system((Compiler[i].dependency+" >/dev/null 2>/dev/null").c_str()))==0)
            Compiler[i].usable=1;
    }
}

static int Compile(string pathto,string problem,int terminal, int optimize) {
    if (!Inited)
        InitCompiler();
    int dot;
    bool found=false;
    for (int i=0;i<Compiler.size();i++) {
        if (exist(pathto+problem+"."+Compiler[i].suffix)&&(Compiler[i].usable)) {
            dot=i;
            found=true;
            break;
        }
    }
    Problem TEMP;
    if (!found)
        return TEMP.SafetyCheck(pathto+problem+"."+Compiler[0].suffix);
    if (TEMP.SafetyCheck(pathto+problem+"."+Compiler[dot].suffix))
        return 1;
    string tmp=Compiler[dot].cmd;
    if (optimize)
        tmp += " -O2";
    if (!terminal)
        tmp += " 2>.ejudge.tmp";
    char str[512];
    sprintf(str,tmp.c_str(),(pathto+problem+"."+Compiler[dot].suffix).c_str(),("./"+problem).c_str());
    int tmpint;
    return WEXITSTATUS(tmpint=system(str));
}
#endif
