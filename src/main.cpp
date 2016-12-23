#include "judge.h"
#include "Functions_Linux.h"
#include <cstdlib>
using namespace std;

int main(int argc,char *argv[]) {
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
