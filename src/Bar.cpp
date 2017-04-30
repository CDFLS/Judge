#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include "Conio.h"
#include "Bar.h"
#include "Functions_Linux.h"
using namespace std;

#define TEXT_MIDDLE 1
#define TEXT_LEFT 2
int Bar::zh_CN_length() {
    int tmp=0;
    for (int i=0;i<text.length();i++)
        if (text[i]<0)
            tmp++;
    return text.length()-tmp/3;
}

Bar::Bar(string TEXT,int X,int Y,int LEN,int Foreground,int Background,int style) {
    init(TEXT,X,Y,LEN,Foreground,Background,style);
}

Bar::Bar() {}

Bar Bar::init(string TEXT,int X,int Y,int LEN,int Foreground,int Background,int style) {
    text=TEXT;x=X;y=Y;len=LEN;fg=Foreground;bg=Background;sty=style;
    return *this;
}
void Bar::show() {
    color(fg,bg);
    gotoxy(x,y);
    if (sty==TEXT_MIDDLE) {
        int SpacesNumber=(len-zh_CN_length())/2;
        for (int cnt=0;cnt<SpacesNumber;cnt++)
            putchar(' ');
        cout << text;
        for (int cnt=0;cnt<len-zh_CN_length()-SpacesNumber;cnt++)
            putchar(' ');
    } else if (sty==TEXT_LEFT) {
        cout << text;
        for (int cnt=0;cnt<len-zh_CN_length();cnt++)
            putchar(' ');
    }
    ClearColor();
}

void Bar::rever() {
    int t=fg;fg=bg;bg=t;
    show();
    t=fg;fg=bg;bg=t;
}
