#ifndef JUDGE_CONFIG_H
#define JUDGE_CONFIG_H
#include <cstdio>
#include <iostream>
#include <fstream>
#include "judge.h"
using namespace std;

namespace Config {
    void FPrintResult(ofstream &fout, JudgeResult *q);
    void FScanfResult(ifstream &fin, JudgeResult *q);
    void Saveto(Contest *x, string filename);
    void Readfrom(Contest *x, string filename);
};
#endif
