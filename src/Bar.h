#ifndef JUDGE_BAR_H
#define JUDGE_BAR_H
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include "Conio.h"
#include "Functions_Linux.h"
using namespace std;

#define TEXT_MIDDLE 1
#define TEXT_LEFT 2
class Bar {
    private:
        int zh_CN_length();
    public:
        Bar(string TEXT,int X,int Y,int LEN,int Foreground,int Background,int style);
        Bar();
        string text;
        int x,y,len,fg,bg,sty;
        Bar init(string TEXT,int X,int Y,int LEN,int Foreground,int Background,int style);
        void show();
        void rever();
};
#endif
