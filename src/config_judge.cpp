#include <cstdio>
#include <iostream>
#include <fstream>
#include "judge.h"
#include "config_judge.h"
using namespace std;

namespace Config {
    void FPrintResult(ofstream &fout, JudgeResult *q) {
        fout << q->score << endl
             << q->st << endl
             << q->time << endl
             << q->memo << endl
             << q->ExtraInfo << '\\' << endl;
    }

    void FScanfResult(ifstream &fin, JudgeResult *q) {
        fin >> q->score
            >> q->st
            >> q->time
            >> q->memo;
        getline(fin,q->ExtraInfo,'\n');//Ignore the first '\n'
        getline(fin,q->ExtraInfo,'\\');
    }

    void Saveto(Contest *x, string filename) {
        ofstream fout;
        fout.open(filename);
        fout << x->problem.size() << endl;
        for (int i=0;i<x->problem.size();i++) {
            Problem *p=&x->problem[i];
            fout << p->name_to_print << endl
                 << p->name << endl
                 << p->memorylimit << endl
                 << p->timelimit << endl
                 << p->eachscore << endl
                 << p->point.size() << endl;
            for (int j=0;j<p->point.size();j++) {
                TestPoint *q=&p->point[j];
                fout << q->stdInput << endl
                     << q->stdOutput << endl;
            }
        }
        fout << x->oier.size() << endl;
        for (int i=0;i<x->oier.size();i++) {
            Contestant *p=&x->oier[i];
            fout << p->name_to_print << endl
                << p->name << endl
                << p->sum << endl
                << p->problem.size() << endl;
            for (int j=0;j<p->problem.size();j++) {
                JudgeResult *q=&p->problem[j];
                FPrintResult(fout,q);
                fout << q->subresult.size() << endl;
                for (int k=0;k<q->subresult.size();k++)
                    FPrintResult(fout,&q->subresult[k]);
            }
        }
        fout.close();
    }

    void Readfrom(Contest *x, string filename) {
        ifstream fin;
        fin.open(filename);
        int size;
        fin >> size;
        while (x->problem.size()<size)
            x->problem.push_back((Problem){});
        for (int i=0;i<x->problem.size();i++) {
            Problem *p=&x->problem[i];
            fin >> p->name_to_print
                >> p->name
                >> p->memorylimit
                >> p->timelimit
                >> p->eachscore
                >> size;
            while (p->point.size()<size)
                p->point.push_back((TestPoint){});
            for (int j=0;j<p->point.size();j++) {
                TestPoint *q=&p->point[j];
                fin >> q->stdInput
                    >> q->stdOutput;
            }
        }
        fin >> size;
        while (x->oier.size()<size)
            x->oier.push_back((Contestant){});
        for (int i=0;i<x->oier.size();i++) {
            Contestant *p=&x->oier[i];
            fin >> p->name_to_print
                >> p->name
                >> p->sum
                >> size;
            while (p->problem.size()<size)
                p->problem.push_back((JudgeResult){});
            for (int j=0;j<p->problem.size();j++) {
                JudgeResult *q=&p->problem[j];
                FScanfResult(fin,q);
                fin >> size;
                while (q->subresult.size()<size)
                    q->subresult.push_back((JudgeResult){});
                for (int k=0;k<q->subresult.size();k++)
                    FScanfResult(fin,&q->subresult[k]);
            }
        }
        fin.close();
    }
};
