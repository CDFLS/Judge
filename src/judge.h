#ifndef JUDGE_H
#define JUDGE_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include "Conio.h"
#include "TrieTree.h"
using namespace std;

#define AC 0
#define WA 1
#define CE 2
#define TLE 3
#define MLE 4
#define RE 5
#define FE 6

class Contest;

namespace JudgeSettings {
    static char Status[][40]={"Accepted","Wrong Answer","Compile Error","Time Limit Exceeded","Memory Limit Exceeded","Runtime Error","File Error"};
    static char Status_Short[]="AW-TMRF";
    static int Status_Backround=white;
    static int Status_Color[]={green,red,yellow,red,red,yellow,yellow};//输出Accepted等提示信息的颜色
    static TrieTree Func, Header, Const;
    //Function模式会检测括号，Header模式只在include中查询，Const模式直接匹配。都会忽略空格。
    static double Default_timelimit=1;
    static int Default_memorylimit=128000;
    static int FileIO=0;
    static string PrintDevice;
    static int use_freopen=0,Terminal=1,PrinttoCSV=-1,UseCUI=0;
    int ConverttoInt(string colorname);
    void SourceProblem(string name,ifstream &fin,Contest *x);
    void ReadSettings(const char *settingsfile,Contest *x);
    int ReadFromArgv(int c,char *v[]);
    void InitTrieTree();
};

class JudgeResult{
    public:
        int st,memo;
        double time;
        int score;
        vector<JudgeResult> subresult;
        string ExtraInfo;
};

class TestPoint {
    public:
        string stdInput,stdOutput;
        JudgeResult JudgePoint(string bin,double timelimit,int memorylimit,int MaxScore,string &Directory);
        bool operator < (TestPoint x);
};

class Contestant {
    public:
        string name,name_to_print;
        vector<JudgeResult> problem;
        int sum;
        void sumup();
        bool operator < (const Contestant &x) const;
};

class Problem {
    public:
        string name,name_to_print;
        vector<TestPoint> point;
        int memorylimit=128000;
        double timelimit=1;
        int eachscore=0;
        void InitProblem();
        bool SafetyCheck(string filename);
        JudgeResult JudgeProblem(Contestant &oier);
};

class Contest {
    public:
        vector<Contestant> oier;
        vector<Problem> problem;
        void InitSPJ();
        void InitContest();
        void JudgeContest();
        void Judge_CUI();
};

namespace JudgeOutput {
    void PrintStatus(int st);
    void PrintResult(JudgeResult &x);
    void PrintError();
    void Print_zh_CN(string str,int len);
    void OutputContest(Contest &test);
    void ConverttoCSV(Contest &test,string csv);
};
#endif
