#include "ConfGen.h"
#include <fstream>

string colorstr(int color) {
    if (color == black)
        return "black";
    else if (color == blue)
        return "blue";
    else if (color == green)
        return "green";
    else if (color == cyan)
        return "cyan";
    else if (color == red)
        return "red";
    else if (color == purple)
        return "purple";
    else if (color == yellow)
        return "yellow";
    else if (color == white)
        return "white";
    else
        return "black";
}

string boolstr(int b) {
    if (b == 1)
        return "true";
    else
        return "false";
}

string encode(string s) {
    string ret;
    for (int i = 0; i < s.length(); ++ i) {
        if (s[i] == ',' || s[i] == ')' || s[i] == '(')
            ret.push_back('\\');
        ret.push_back(s[i]);
    }
    return ret;
}

#ifdef EN
    #define INFO1 "Do you want to use file I/O ? [Y/n]"
    #define INFO2 "Add -O2 option when compiling ? [Y/n]"
    #define INFO3 "Finished."
#else
    #define INFO1 "是否使用文件输入输出？ [Y/n]"
    #define INFO2 "编译时加入 -O2 选项？ [Y/n]"
    #define INFO3 "已生成配置。"
#endif

void GenConf(JudgeSettings *conf, Contest* x) {
    cout << INFO1;
    char ch = 0;
    while (ch != 'Y' && ch != 'y' && ch != 'N' && ch != 'n')
        ch = getch();
    if (ch == 'Y' || ch == 'y')
        conf->FileIO = 1;
    else
        conf->FileIO = -1;
    cout << endl << INFO2;
    ch = 0;
    while (ch != 'Y' && ch != 'y' && ch != 'N' && ch != 'n')
        ch = getch();
    if (ch == 'Y' || ch == 'y')
        conf->O2opti = 1;
    else
        conf->O2opti = 0;
    ofstream fout;
    fout.open("judge.conf");
    fout << "background(" << colorstr(conf->Status_Background) << ")" << endl;
    fout << "FileIO(" << boolstr(conf->FileIO) << ")" << endl;
    fout << "O2(" << boolstr(conf->O2opti) << ")" << endl << endl;
    for (auto i : x->problem) {
        fout << "enter(" << encode(i.name_to_print) << ")" << endl;
        fout << "    name(" << encode(i.name_to_print) << ")" << endl;
        fout << "    time(" << i.timelimit << ")" << endl;
        fout << "    memo(" << i.memorylimit << ")" << endl;
        fout << "    score(" << i.eachscore << ")" << endl;
        fout << "quit()" << endl;
        fout << endl;
    }
    fout.close();
    cout << endl << INFO3 << endl;
}
