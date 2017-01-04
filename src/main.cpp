#include "judge.h"
#include "Functions_Linux.h"
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "sandbox/run.h"
#include "sandbox/execute.h"
using namespace std;

static void parse_arguments(int argc, char *argv[]) {
    int opt;
    extern char *optarg;

    while ((opt = getopt(argc, argv, "c:t:m:d:s:i:o:")) != -1) {
        switch (opt) {
            case 'c':
                strncpy(codepath, optarg, 120);
                break;
            case 'i':
                strncpy(in_file, optarg, 120);
                break;
            case 'o':
                strncpy(out_file, optarg, 120);
                break;
            case 't':
                time_limit = atoi(optarg);
                break;
            case 'm':
                memory_limit = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Unknown option -%c\n", opt);
                exit(-1);
        }
    }
}

int SandBoxRun(int argc, char *argv[]) {
    parse_arguments(argc, argv);
    int mem = 0;
    double time = 0;
	freopen("/dev/null","w",stderr);
    int ret = execute(in_file, out_file, codepath, time_limit, memory_limit,
                      30000, &mem, &time);
	freopen("/dev/stderr","w",stderr);
    fprintf(stdout, "%d %lf %d",-ret, time, mem);
    return 0;
}

int main(int argc,char *argv[]) {
	if (argc>1&&(string)argv[1]=="exec") {
		SandBoxRun(argc,argv);
		return 0;
	}
	if (argc==4&&(string)argv[1]=="python") {
		system(("echo \"#!/bin/python\" > "+(string)argv[3]).c_str());
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
