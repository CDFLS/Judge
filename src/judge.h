#ifndef JUDGE_H
#define JUDGE_H 

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include "Conio.h"
using namespace std;

#define AC 0
#define WA 1
#define CE 2
#define TLE 3
#define MLE 4
#define RE 5

namespace JudgeSettings {
	static char Status[][40]={"Accepted","Wrong Answer","Compile Error","Time Limit Exceeded","Memory Limit Exceeded","Runtime Error"};
	static char Status_Short[]="AW-TMR";
	static int Status_Backround=white;
	static int Status_Color[]={green,red,yellow,red,red,yellow};//输出Accepted等提示信息的颜色
	static vector<string> InvalidWords={"system(","rand()"},InvalidHeaders={"windows.h","con","/dev/tty"};
	static double Default_timelimit=1;
	static int Default_memorylimit=128000;
	static string PrintDevice;
	static int use_freopen=0,Terminal=1,PrinttoCSV=-1,UseCUI=0;
	int ConverttoInt(string colorname);
	void ReadSettings(const char *settingsfile);
	int ReadFromArgv(int c,char *v[]);
};

class JudgeResult{
	public:
	    int st,memo;
	    double time;
		int score;
		vector<JudgeResult> subresult;
};

class TestPoint {
	public:
		string stdInput,stdOutput;
		int MaxScore;
		JudgeResult JudgePoint(string bin,double timelimit,int memorylimit);
		JudgeResult JudgePointSPJ(string bin,string spj,double timelimit,int memorylimit);
};

class Contestant {
	public:
		string name,name_to_print;
		vector<JudgeResult> problem;
		int sum;
		void sumup();
		bool operator < (Contestant &x);
};

class Problem {
	public:
		string name,name_to_print;
		vector<TestPoint> point;
		int memorylimit=128000;
		double timelimit=1;
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
