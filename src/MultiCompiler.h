#ifndef MULTICOMPILER_H
#define MULTICOMPILER_H
#include "judge.h"
#include <iostream>
#include <string>
using namespace std;
vector<string> SupportedFile={"cpp","c"};
vector<string> CompileCommand={
	"g++ -static -lm -s -DEJUDGE %s -o %s",
	"gcc -static -lm -s -DEJUDGE %s -o %s",
};
int Compile(string pathto,string problem,int terminal) {
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
