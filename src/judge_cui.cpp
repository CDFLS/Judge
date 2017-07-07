#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "Bar.h"
#include "judge.h"
#include "ConverttoString.h"
#include "ConfigJudge.h"
#include "MultiCompiler.h"
#include "Functions_Linux.h"

#ifdef EN
#include "lang/language_en.h"
#else
#include "lang/language.h"
#endif

using namespace std;

int W, L;

int zhCNExt(string str) {
    int tmp = 0;
    for (int i = 0; i < str.length();i ++)
        if (str[i] < 0)
            tmp ++;
    return tmp / 3;
}

int maxl;

string info(Contestant *x, int MAXL = maxl) {
    char str[256], tmp[256];
    sprintf(tmp, "%%%ds", MAXL + 3 + zhCNExt(x->name_to_print));
    sprintf(str, tmp, x->name_to_print.c_str());
    string info = str;
    sprintf(str, "%6d", x->sum);
    info += str;
    for (int j = 0; j < x->problem.size(); j++) {
        sprintf(str, "%6d", x->problem[j].score);
        info += str;
    }
    return info;
}

#define TITLE 0
#define MAINWINDOW 1
#define MENU 2
#define PAGES 2
#define BLANKS 2
#define BOTTOM 3
#define FG(x) ColorScheme::c[x].first
#define BG(x) ColorScheme::c[x].second
#define PAIR(x) ColorScheme::c[x].first, ColorScheme::c[x].second

namespace ColorScheme {
    static vector<pair<int, int> > c={
        {white, blue},                    //Title
        {black, white},                   //MAINWINDOW
        {white, blue},                    //MENU
        {white, blue},                    //BOTTOM
    };
};

vector<Bar> Menu;
int Tab = 0, Select = 0;
vector<Bar> Blanks;
vector<Bar> ShowPage;
Bar Title;

inline int page() {
    return Select / L;
}

int maxpage;
void ShowContestant(vector<Contestant> &x, int k, int rev) {
    if (k >= x.size()) {
        color(PAIR(MAINWINDOW));
        gotoxy(11, k % L + 2);
        for (int i = 0; i < W - 10; ++ i)
            putchar(' ');
        return ;
    }
    string tmp;
    if (rev)
        color(BG(MAINWINDOW), FG(MAINWINDOW));
    else
        color(PAIR(MAINWINDOW));
    gotoxy(11, k % L + 2);
    cout << (tmp = info(&x[k]));
    for (int i = 0; i < W - 10 - (tmp.length() - zhCNExt(tmp)); ++ i)
        putchar(' ');
}

void ShowContestants(vector<Contestant> &x) {
    for (int i = L * page(); i < L * (page() + 1); ++ i)
        ShowContestant(x, i, 0);
    ShowContestant(x, Select, 1);
}

void ShowMenu() {
    for (auto i : Blanks)
        i.show();
    for (auto i : ShowPage)
        i.show();
    Title.show();
    for (auto i : Menu)
        i.show();
    Menu[Tab].rever();
    ShowPage[page()].rever();
}

void Exit(Contest* x) {
    Config::Saveto(x, "judge.log");
    unhidecursor();
    ClearColor();
    puts("");
    system("clear");
}

void Contest::Judge_CUI() {
    struct winsize size;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
    W = size.ws_col - 9;
    L = size.ws_row - 2;
    //experimental feature
    if (exist("judgecolor.conf")) {
        ifstream fin;
        fin.open("judgecolor.conf");
        string l1, l2;
        for (auto i : ColorScheme::c) {
            fin >> l1 >> l2;
            i.first = conf->ConverttoInt(l1);
            i.second = conf->ConverttoInt(l2);
        }
        fin.close();
    }
    sort(oier.begin(), oier.end());
    color(PAIR(TITLE));
    system("clear");
    hidecursor();

    for (auto i : oier)
        maxl = max(maxl, (int)i.name_to_print.length() - zhCNExt(i.name_to_print));

    Menu.push_back(Bar(Context::View, 1, 2, 10, PAIR(MENU), TEXT_MIDDLE));
    Menu.push_back(Bar(Context::Rejudge, 1, 3, 10, PAIR(MENU), TEXT_MIDDLE));
    Blanks.push_back(Bar(Context::CUIhelp, 1, L + 2, W + 9, PAIR(BOTTOM), TEXT_MIDDLE));
    for (int i = Menu.size() + 2; i <= L + 1; ++ i)
        Blanks.push_back(Bar("", 1, i, 10, PAIR(BLANKS), TEXT_LEFT));
    Title = Bar(" Total", 1, 1, W + 9, PAIR(TITLE), TEXT_LEFT);
    for (auto i : problem) {
        char str[256];
        sprintf(str, " %5s", i.name_to_print.c_str());
        Title.text += str;
    }
    for (int i = 0; i < maxl + 8; ++ i)
        Title.text = " " + Title.text;
    Title.text = "Judge" + Title.text;

    maxpage = oier.size() / L + 1 - ((oier.size() % L) ? 0: 1);
    for (int i = 0; i < maxpage; ++ i)
        ShowPage.push_back(Bar(CTS::IntToString(i + 1), W + 1, i + 2, 9, PAIR(PAGES), TEXT_MIDDLE));
    for (int i = ShowPage.size() + 2; i <= L + 1; ++ i)
        Blanks.push_back(Bar("", W + 1, i, 9, PAIR(BLANKS), TEXT_LEFT));

    ShowMenu();
    ShowContestants(oier);

    while (true) {
        char ch = getch();
        int lasts = Select;
        int lastpage = page();
        int lasttab = Tab;
        if (ch == 'w' || ch == 'A')
            Select --;
        if (ch == 's' || ch == 'B')
            Select ++;
        if (ch == 'a' || ch == 'D') {
            int tmp = page();
            tmp --;
            if (tmp == -1)
                tmp = maxpage - 1;
            Select = tmp * L;
        }
        if (ch == 'd' || ch == 'C') {
            int tmp = page();
            tmp ++;
            if (tmp == maxpage)
                tmp = 0;
            Select = tmp * L;
        }
        if (ch == '\t')
            Tab = (Tab + 1) % Menu.size();
        if (ch == 'q') {
            Exit(this);
            return ;
        }
        while (Select < 0)
            Select += oier.size();
        while (Select >= oier.size())
            Select -= oier.size();
        if (page() != lastpage) {
            ShowContestants(oier);
        } else if (Select != lasts) {
            ShowContestant(oier, Select, 1);
            ShowContestant(oier, lasts, 0);
        }
        if (lastpage != page()) {
            ShowPage[page()].rever();
            ShowPage[lastpage].show();
        }
        if (lasttab != Tab) {
            Menu[Tab].rever();
            Menu[lasttab].show();
        }

        if ((ch == '\n' || ch == '\r') && (Tab == 1)) {
            Contestant *P = &oier[Select];
            ClearColor();
            puts("");
            system("clear");
            for (int i=0;i<problem.size();i++) {
                cout << Context::Contestant << ": " << P->name_to_print << " "+Context::Problem+" " << i+1 << ": " << problem[i].name_to_print << " [" << i+1 << '/' << problem.size() << ']' << endl;
                P->problem[i]=problem[i].JudgeProblem(oier[Select]);
                printf("\n%s ",P->name_to_print.c_str());
                JudgeOutput::PrintResult(P->problem[i]);
                printf("%s: %d\n\n",Context::Score,P->problem[i].score);
            }
            Context::PressToContinue();
            system("clear");
            P->sumup();
            stable_sort(oier.begin(),oier.end());
            ShowMenu();
            ShowContestants(oier);
        }

        if ((ch == '\n' || ch == '\r') && (Tab == 0)) {
            Contestant *P = &oier[Select];
            ClearColor();
            puts("");
            system("clear");
            for (int i=0;i<problem.size();i++) {
                cout << Context::Contestant << ": " << P->name_to_print << " "+Context::Problem+" " << i+1 << ": " << problem[i].name_to_print << " [" << i+1 << '/' << problem.size() << ']' << endl;
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
                    Compile(P->name, problem[i].name_to_print, 1, conf->O2opti);
                JudgeOutput::PrintResult(P->problem[i]);
                printf("%s: %d\n\n",Context::Score,P->problem[i].score);
            }
            Context::PressToContinue();
            system("clear");
            P->sumup();
            stable_sort(oier.begin(),oier.end());
            ShowMenu();
            ShowContestants(oier);
        }

        //ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
        //if (W != size.ws_col - 9 || L != size.ws_row - 2) {
            //W = size.ws_col - 9;
            //L = size.ws_row - 2;
            //ShowContestants(oier);
            //ShowMenu();
        //}
        //TODO: Auto resize
    }
    Exit(this);
}
