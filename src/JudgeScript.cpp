#include <fstream>

#include "JudgeScript.h"
#include "judge.h"
#include "Conio.h"
#include "interpreter/AST.h"

#ifdef EN
#include "lang/language_en.h"
#else
#include "lang/language.h"
#endif

#define Def(x) Array x (unordered_map<string, Any>* ast, Array arr)
#define True Array(1, 1)
#define False Array(1, 0)
Contest* contest;

Def(JudgeProblem) {
    int i = arr[0].Int();
    int pro = arr[1].Int();
    Contestant *P = &contest->oier[i];
    cout << Context::Contestant << ": " << P->name_to_print << " "+Context::Problem+" " << pro + 1 << ": " << contest->problem[pro].name_to_print << " [" << pro + 1 << '/' << contest->problem.size() << ']' << endl;
    P->problem[pro] = contest->problem[pro].JudgeProblem(*P);
    printf("\n%s ", P->name_to_print.c_str());
    JudgeOutput::PrintResult(P->problem[pro]);
    printf("%s: %d\n\n", Context::Score, P->problem[pro].score);
    P->sumup();
    return Array(1, 0);
}

unordered_map<string, int> Oier;
unordered_map<string, int> Problem;

Def(GetContestant) {
    string name = arr[0].Str();
    return Array(1, Oier[name]);
}

Def(GetProblem) {
    string name = arr[0].Str();
    return Array(1, Problem[name]);
}

Def(ContestantSize) {
    return Array(1, (int)contest->oier.size());
}

Def(ProblemSize) {
    return Array(1, (int)contest->problem.size());
}

Def(GetContestantName) {
    return Array(1, contest->oier[arr[0].Int()].name_to_print);
}

Def(GetProblemName) {
    return Array(1, contest->problem[arr[0].Int()].name_to_print);
}

Def(PrintToScreen) {
    JudgeOutput::OutputContest(*contest);
    return Array(1, 0);
}

Def(PrintToCSV) {
    JudgeOutput::ConverttoCSV(*contest, arr[0].Str());
    return Array(1, 0);
}

void InitTree(AST& rt) {
    for (int i = 0; i < contest->oier.size(); ++ i)
        Oier[contest->oier[i].name_to_print] = i;
    for (int i = 0; i < contest->problem.size(); ++ i)
        Problem[contest->problem[i].name_to_print] = i;
    rt.Add("judge", Any(JudgeProblem));
    rt.Add("oier.find", Any(GetContestant));
    rt.Add("problem.find", Any(GetProblem));
    rt.Add("oier.size", Any(ContestantSize));
    rt.Add("problem.size", Any(ProblemSize));
    rt.Add("oier.name", Any(GetContestantName));
    rt.Add("problem.name", Any(GetProblemName));
    rt.Add("contest.print", Any(PrintToScreen));
    rt.Add("contest.csv", Any(PrintToCSV));
}

void ExecuteScript(Contest* x, string filename) {
    contest = x;
    AST rt;
    InitTree(rt);
    ifstream fin;
    string tmp;
    fin.open(filename);
    if (!fin)
        cerr << "Error: file not found: " << filename << endl;
    fin >> tmp;
    if (tmp == "(main)")
        return ;
    if (tmp != "(main")
        cerr << "Error: function 'main' not defined." << endl;
    rt.Build("main", fin);
    rt.Exec();
    fin.close();
}
