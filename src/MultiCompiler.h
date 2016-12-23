#ifndef MULTICOMPILER_H
#define MULTICOMPILER_H
#include "judge.h"
#include <iostream>
#include <string>
using namespace std;
static vector<string> SupportedFile={"cpp","c","pas","py"};
static vector<string> CompileCommand={
	"timeout 3s g++ -static -std=c++11 -s -Wall -O2 -lm -DEJUDGE %s -o Exec/%s",
	"timeout 3s gcc -static -std=c99 -fno-asm -s -Wall -O2 -lm -DEJUDGE %s -o Exec/%s",
	"timeout 3s fpc -Xs -Sgic -O2 -dEJUDGE %s -oExec/%s"
};
static int Compile(string pathto,string problem,int terminal) {
	int dot;
	bool found=false;
	for (int i=0;i<SupportedFile.size();i++)
		if (exist(pathto+problem+"."+SupportedFile[i])) {
			dot=i;
			found=true;
			break;
		}
	Problem TEMP;
	if (!found)
		return TEMP.SafetyCheck(pathto+problem+"."+SupportedFile[0]);
	if (TEMP.SafetyCheck(pathto+problem+"."+SupportedFile[dot]))
		return 1;
	string tmp=CompileCommand[dot];
	if (!terminal)
		tmp+=" 2>.ejudge.tmp";
	char str[512];
	sprintf(str,tmp.c_str(),(pathto+problem+"."+SupportedFile[dot]).c_str(),("./"+problem).c_str());
	return WEXITSTATUS(system(str));
}
#endif
