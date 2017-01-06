#include "judge.h"
#include "Functions_Linux.h"
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "sandbox/execute.h"
using namespace std;

int main(int argc,char *argv[]) {
	if (argc==4&&(string)argv[1]=="python") {
		system(("echo \"#!/bin/python\" > "+(string)argv[3]).c_str());
		system(("cat "+(string)argv[2]+" >> "+(string)argv[3]).c_str());
		system(("chmod +x "+(string)argv[3]).c_str());
		return 0;
	}
	if (argc==4&&(string)argv[1]=="python2") {
		system(("echo \"#!/bin/python2\" > "+(string)argv[3]).c_str());
		system(("cat "+(string)argv[2]+" >> "+(string)argv[3]).c_str());
		system(("chmod +x "+(string)argv[3]).c_str());
		return 0;
	}
	system("mkdir Exec 2>/dev/null");
	Contest x;
	if (JudgeSettings::ReadFromArgv(argc,argv)) {
		system("rm Exec -rf");
		return 0;
	}
	x.InitContest();
	x.InitSPJ();
	JudgeSettings::ReadSettings(GetJudgerc().c_str(),&x);
	JudgeSettings::ReadSettings("judgerc",&x);
	x.JudgeContest();
	ClearFile();
	JudgeOutput::OutputContest(x);
	system("rm Exec -rf 2>/dev/null");
    return 0;
}
