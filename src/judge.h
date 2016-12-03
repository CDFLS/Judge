#ifndef JUDGE_H
#define JUDGE_H 

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "Conio.h"

#define AC 0
#define WA 1
#define CE 2
#define TLE 3
#define MLE 4
#define RE 5

extern const char Status[][40];
extern const char Status_Color[];
extern char Dict[100][20];
extern int Arg_c;
extern int Flag_Freopen;
extern double timelimit;
extern int memorylimit;
extern char name[512];

struct result{
    int s,memo;
    double time;
};//Judge的结果

extern void print(int st);
extern void PrintResult(result x);
extern bool cmp(char *str,int s,char *word);
extern int Args(int c,char *v[]);
extern bool exist(char * filename);
extern bool SafetyCheck();
extern bool Compile();
extern result judge(char *in,char *out);
extern int judge_single();
#endif
