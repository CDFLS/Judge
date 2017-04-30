#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "judge.h"
#include "Bar.h"
#include "ConverttoString.h"
#include "config_judge.h"
#include "MultiCompiler.h"
#include "Functions_Linux.h"

#ifdef EN
#include "lang/language_en.h"
#else
#include "lang/language.h"
#endif

using namespace std;

static int W,L;
class Bar_oier:public Bar {
    public:
        Contestant *p;
        Bar_oier(string TEXT,int X,int Y,int LEN,int Foreground,int Background,int style,Contestant *oier):Bar(TEXT,X,Y,LEN,Foreground,Background,style) {
            p=oier;
        }
        bool operator < (Bar_oier x) const {
            return this->p->sum > x.p->sum;
        }
};


class Page {
    public:
        vector<Bar_oier> p;
        void print() {
            for (int i=0;i<min((int)p.size(),L);i++)
                p[i].show();
        }
        void print(int page) {
            for (int i=page*L;i<min((int)p.size(),(page+1)*L);i++)
                p[i].show();
        }
};

int zh_CN_exlen(string str) {
    int tmp=0;
    for (int i=0;i<str.length();i++)
        if (str[i]<0)
            tmp++;
    return tmp/3;
}

string Ginfo(Contestant *x,int MAXL) {
    char str[256],tmp[256];
    sprintf(tmp,"%%%ds",MAXL+3+zh_CN_exlen(x->name_to_print));
    sprintf(str,tmp,x->name_to_print.c_str());
    string info=str;
    sprintf(str,"%6d",x->sum);
    info+=str;
    for (int j=0;j<x->problem.size();j++) {
        sprintf(str,"%6d",x->problem[j].score);
        info+=str;
    }
    return info;
}

#define TITLE 0
#define MAINWINDOW 1
#define MENU 2
#define PAGES 2
#define BLANKS 2
#define BOTTOM 3
#define PAIR(x) ColorScheme::c[x].fg,ColorScheme::c[x].bg

struct ColorPair {
    int fg,bg;
};

namespace ColorScheme {
    static vector<ColorPair> c={
        {white,blue},                    //Title
        {black,white},                    //MAINWINDOW
        {white,blue},                    //MENU
        {white,blue},                    //BOTTOM
    };
};

void Contest::Judge_CUI() {
    struct winsize size;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
    W=size.ws_col-9;
    L=size.ws_row-3;

    //experimental, used for choose colorscheme
    if (exist("judgecolor")) {
        ifstream fin;
        fin.open("judgecolor");
        string l1,l2;
        for (int i=0;i<ColorScheme::c.size();i++) {
            fin >> l1 >> l2;
            ColorScheme::c[i].fg=JudgeSettings::ConverttoInt(l1);
            ColorScheme::c[i].bg=JudgeSettings::ConverttoInt(l2);
        }
        fin.close();
    }

    sort(oier.begin(),oier.end());
    color(white,blue);
    system("clear");
    hidecursor();
    Page window;
    vector<Bar> menu=(vector<Bar>){(Bar){Context::View,1,2,10,PAIR(MENU),TEXT_MIDDLE},(Bar){Context::Rejudge,1,3,10,PAIR(MENU),TEXT_MIDDLE}};
    vector<Bar> ShowPage;
    vector<Bar> Blanks;
    Blanks.push_back((Bar){Context::CUIhelp,1,L+2,W+9,PAIR(BOTTOM),TEXT_MIDDLE});
    for (int i=menu.size()+2;i<=L+1;i++)
        Blanks.push_back((Bar){"",1,i,10,PAIR(BLANKS),TEXT_LEFT});
    Bar Title(" Total",1,1,W+9,PAIR(TITLE),TEXT_LEFT);
    for (int i=0;i<problem.size();i++) {
        char str[256];
        sprintf(str," %5s",problem[i].name_to_print.c_str());
        Title.text+=str;
    }
    for (int i=0;i<menu.size();i++)
        menu[i].show();
    int npage=0;
    int MAXL=0;
    for (int i=0;i<oier.size();i++)
        MAXL=max(MAXL,-zh_CN_exlen(oier[i].name_to_print)+(int)oier[i].name_to_print.size());
    for (int i=0;i<MAXL+8;i++)
        Title.text=" "+Title.text;
    Title.text="Judge"+Title.text;
    for (int i=0;i<oier.size();i++)
        window.p.push_back((Bar_oier){Ginfo(&oier[i],MAXL),11,i%L+2,W-10,PAIR(MAINWINDOW),TEXT_LEFT,&oier[i]});
    int tmp=(window.p.size()/L+1-((window.p.size()%L==0)?1:0));
    for (int i=0;i<tmp;i++)
        ShowPage.push_back((Bar){CTS::IntToString(i+1),W+1,i+2,9,PAIR(PAGES),TEXT_MIDDLE});
    for (int i=ShowPage.size()+2;i<=L+1;i++)
        Blanks.push_back((Bar){"",W+1,i,9,PAIR(BLANKS),TEXT_LEFT});
    int page=0,s=0,Tab=0;
    menu[Tab].rever();
    window.print(page);
    window.p[s].rever();
    for (int i=0;i<ShowPage.size();i++)
        ShowPage[i].show();
    for (int i=0;i<Blanks.size();i++)
        Blanks[i].show();
    ShowPage[0].rever();
    Title.show();
    char ch;
    while ((ch=getch())!='q') {
        int refresh_menu=0,refresh_list=0,lastTab=Tab,last=s,pageturned=0,lastpage=page;
        if (ch=='w'||ch=='A')
            s--,refresh_list=1;
        if (ch=='s'||ch=='B')
            s++,refresh_list=1;
        if (ch=='a'||ch=='D')
            page--,pageturned=1;
        if (ch=='d'||ch=='C')
            page++,pageturned=1;
        if ((ch=='\n'||ch=='\r')&&(Tab==1)) {
            Contestant *P=window.p[s+page*L].p;
            system("clear");
            for (int i=0;i<problem.size();i++) {
                cout << Context::Contestant+" " << ':' << P->name_to_print << " "+Context::Problem+" " << i+1 << ':' << problem[i].name_to_print << " [" << i+1 << '/' << problem.size() << ']' << endl;
                P->problem[i]=problem[i].JudgeProblem(oier[s]);
                printf("\n%s ",P->name_to_print.c_str());
                JudgeOutput::PrintResult(P->problem[i]);
                printf("%s:%d\n\n",Context::Score,P->problem[i].score);
            }
            Context::PressToContinue();
            system("clear");
            P->sumup();
            window.p[s].text=Ginfo(P,MAXL);
            stable_sort(window.p.begin(),window.p.end());
            for (int i=0;i<window.p.size();i++)
                window.p[i].y=(i+1)%L+1;
            window.print(page);
            window.p[s].rever();
            for (int i=0;i<menu.size();i++)
                menu[i].show();
            menu[Tab].rever();
            for (int i=0;i<ShowPage.size();i++)
                ShowPage[i].show();
            for (int i=0;i<Blanks.size();i++)
                Blanks[i].show();
            ShowPage[page].rever();
            Title.show();
        }
        if ((ch=='\n'||ch=='\r')&&(Tab==0)) {
            system("clear");
            Contestant *P=window.p[s+page*L].p;
            for (int i=0;i<problem.size();i++) {
                cout << Context::Contestant+" " << ':' << P->name_to_print << " "+Context::Problem+" " << i+1 << ':' << problem[i].name_to_print << " [" << i+1 << '/' << problem.size() << ']' << endl;
                int maxlength=0;
                for (int j=0;j<problem[i].point.size();j++)
                    maxlength=max(maxlength,(int)BaseName(problem[i].point[j].stdInput).length());
                char tmp[256];
                sprintf(tmp,"%%%ds ",maxlength+3);
                if (P->problem[i].st!=CE)
                    for (int j=0;j<problem[i].point.size();j++) {
                        printf(tmp,BaseName(problem[i].point[j].stdInput).c_str());
                        JudgeOutput::PrintResult(P->problem[i].subresult[j]);
                    }
                else
                    Compile(P->name,problem[i].name_to_print,1);
                JudgeOutput::PrintResult(P->problem[i]);
                printf("%s:%d\n\n",Context::Score,P->problem[i].score);
            }
            Context::PressToContinue();
            system("clear");
            P->sumup();
            window.p[s].text=Ginfo(window.p[s].p,MAXL);
            stable_sort(window.p.begin(),window.p.end());
            for (int i=0;i<window.p.size();i++)
                window.p[i].y=(i+1)%L+1;
            window.print(page);
            window.p[s].rever();
            for (int i=0;i<menu.size();i++)
                menu[i].show();
            menu[Tab].rever();
            for (int i=0;i<ShowPage.size();i++)
                ShowPage[i].show();
            for (int i=0;i<Blanks.size();i++)
                Blanks[i].show();
            ShowPage[page].rever();
            Title.show();
        }
        if (ch=='\t')
            Tab++,refresh_menu=1;
        if (refresh_menu) {
            Tab%=menu.size();
            menu[lastTab].show();
            menu[Tab].rever();
        }
        if (pageturned) {
            int tmp=window.p.size()/L+1-((window.p.size()%L==0)?1:0);
            page=(page+tmp)%tmp;
            system("clear");
            for (int i=0;i<menu.size();i++)
                menu[i].show();
            for (int i=0;i<ShowPage.size();i++)
                ShowPage[i].show();
            for (int i=0;i<Blanks.size();i++)
                Blanks[i].show();
            ShowPage[page].rever();
            menu[Tab].rever();
            window.print(page);
            Title.show();
            s=0;
            refresh_list=1;
        }
        if (refresh_list) {
            window.p[last+page*L].show();
            int tmp=min((int)window.p.size()-page*L,L);
            s=(s+tmp)%tmp;
            window.p[s+page*L].rever();
        }
    }
    unhidecursor();
    system("clear");
    Config::Saveto(this,"judgelog");
    return ;
}
