#include "judge.h"
#include "Functions_Linux.h"
using namespace std;

int main(int argc,char *argv[]) {
    GetName(name);
    int score=0;
    if (!(Args(argc,argv)||Compile()))
        score=judge_single();
    ClearColor();
    ClearFile();
    return score;
}
