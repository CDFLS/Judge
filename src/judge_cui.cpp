#include "judge.h"
#include "Bar.h"
#include "ConverttoString.h"
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <iostream>

#ifdef EN
#include "lang/language_en.h"
#else
#include "lang/language.h"
#endif

using namespace std;

class Bar_oier:public Bar {
	public:
		Contestant *p;
		Bar_oier(string TEXT,int X,int Y,int LEN,int Foreground,int Background,int style,Contestant *oier):Bar(TEXT,X,Y,LEN,Foreground,Background,style) {
			p=oier;
		}
		bool operator < (Bar_oier x) const {
			return this->p->sum > x.p->sum;
		}
};

class Page {
	public:
		vector<Bar_oier> p;
		void print() {
			for (int i=0;i<min((int)p.size(),24);i++)
				p[i].show();
		}
		void print(int page) {
			for (int i=page*24;i<min((int)p.size(),(page+1)*24);i++)
				p[i].show();
		}
};

static int W=71,L=24;

int zh_CN_exlen(string str) {
	int tmp=0;
	for (int i=0;i<str.length();i++)
		if (str[i]<0)
			tmp++;
	return tmp/3;
}

string Ginfo(Contestant *x,int MAXL) {
	char str[256],tmp[256];
	sprintf(tmp,"%%%ds",MAXL+3+zh_CN_exlen(x->name_to_print));
	sprintf(str,tmp,x->name_to_print.c_str());
	string info=str;
	for (int j=0;j<x->problem.size();j++) {
		sprintf(str,"%4d",x->problem[j].score);
		info+=str;
	}
	return info;
}

void Contest::Judge_CUI() {
	sort(oier.begin(),oier.end());
	color(white,yellow);
	system("clear");
	HideCursor();
	Page window;
	vector<Bar> menu=(vector<Bar>){(Bar){Context::View,1,1,10,white,yellow,TEXT_MIDDLE},(Bar){Context::Rejudge,1,2,10,white,yellow,TEXT_MIDDLE}};
	vector<Bar> ShowPage;
	for (int i=0;i<menu.size();i++)
		menu[i].show();
	int npage=0;
	int MAXL=0;
	for (int i=0;i<oier.size();i++)
		MAXL=max(MAXL,zh_CN_exlen(oier[i].name_to_print)+(int)oier[i].name_to_print.size());
	for (int i=0;i<oier.size();i++)
		window.p.push_back((Bar_oier){Ginfo(&oier[i],MAXL),11,i%L+1,W-10,blue,white,TEXT_LEFT,&oier[i]});
	int tmp=(window.p.size()/24+1-((window.p.size()%24==0)?1:0));
	for (int i=0;i<tmp;i++)
		ShowPage.push_back((Bar){CTS::IntToString(i+1),W+1,i+1,9,blue,white,TEXT_MIDDLE});
	int page=0,s=0,tab=0;
	menu[tab].rever();
	window.print(page);
	window.p[s].rever();
	for (int i=0;i<ShowPage.size();i++)
		ShowPage[i].show();
	ShowPage[0].rever();
	char ch;
	while ((ch=getch())!='q') {
		int refresh_menu=0,refresh_list=0,lasttab=tab,last=s,pageturned=0,lastpage=page;
		if (ch=='w')
			s--,refresh_list=1;
		if (ch=='s')
			s++,refresh_list=1;
		if (ch=='a')
			page--,pageturned=1;
		if (ch=='d')
			page++,pageturned=1;
		if ((ch=='\n'||ch=='\r')&&(tab==1)) {
			Contestant *P=window.p[s+page*24].p;
			system("clear");
			for (int i=0;i<problem.size();i++) {
				cout << Context::Contestant+" " << ':' << P->name_to_print << " "+Context::Problem+" " << i+1 << ':' << problem[i].name_to_print << " [" << i+1 << '/' << problem.size() << ']' << endl;
				P->problem[i]=problem[i].JudgeProblem(oier[s]);
				printf("\n%s ",P->name_to_print.c_str());
				JudgeOutput::PrintResult(P->problem[i]);
				printf("%s:%d\n\n",Context::Score,P->problem[i].score);
			}
			Context::PressToContinue();
			system("clear");
			P->sumup();
			window.p[s].text=Ginfo(P,MAXL);
			stable_sort(window.p.begin(),window.p.end());
			for (int i=0;i<window.p.size();i++)
				window.p[i].y=(i+1)%L;
			window.print(page);
			window.p[s].rever();
			for (int i=0;i<menu.size();i++)
				menu[i].show();
			menu[tab].rever();
			for (int i=0;i<ShowPage.size();i++)
				ShowPage[i].show();
			ShowPage[page].rever();
		}
		if ((ch=='\n'||ch=='\r')&&(tab==0)) {
			system("clear");
			Contestant *P=window.p[s+page*24].p;
			for (int i=0;i<problem.size();i++) {
				cout << Context::Contestant+" " << ':' << P->name_to_print << " "+Context::Problem+" " << i+1 << ':' << problem[i].name_to_print << " [" << i+1 << '/' << problem.size() << ']' << endl;
				int maxlength=0;
				for (int j=0;j<problem[i].point.size();j++)
					if ((int)problem[i].point[j].stdInput.size()>maxlength)
						maxlength=problem[i].point[j].stdInput.size();
				char tmp[256];
				sprintf(tmp,"%%%ds ",maxlength+3);
				if (P->problem[i].st!=CE)
					for (int j=0;j<problem[i].point.size();j++) {
						printf(tmp,problem[i].point[j].stdInput.c_str());
						JudgeOutput::PrintResult(P->problem[i].subresult[j]);
					}
				else {
					if (!problem[i].SafetyCheck(P->name+problem[i].name_to_print+".cpp"))
						system(("g++ "+P->name+problem[i].name_to_print+".cpp -o .ejudge.tmp").c_str());
				}
				printf("\n%s ",P->name_to_print.c_str());
				JudgeOutput::PrintResult(P->problem[i]);
				printf("%s:%d\n\n",Context::Score,P->problem[i].score);
			}
			Context::PressToContinue();
			system("clear");
			P->sumup();
			window.p[s].text=Ginfo(window.p[s].p,MAXL);
			stable_sort(window.p.begin(),window.p.end());
			for (int i=0;i<window.p.size();i++)
				window.p[i].y=(i+1)%L;
			window.print(page);
			window.p[s].rever();
			for (int i=0;i<menu.size();i++)
				menu[i].show();
			menu[tab].rever();
			for (int i=0;i<ShowPage.size();i++)
				ShowPage[i].show();
			ShowPage[page].rever();
		}
		if (ch=='\t')
			tab++,refresh_menu=1;
		if (refresh_menu) {
			tab%=menu.size();
			menu[lasttab].show();
			menu[tab].rever();
		}
		if (pageturned) {
			int tmp=window.p.size()/24+1-((window.p.size()%24==0)?1:0);
			page=(page+tmp)%tmp;
			system("clear");
			for (int i=0;i<menu.size();i++)
				menu[i].show();
			for (int i=0;i<ShowPage.size();i++)
				ShowPage[i].show();
			ShowPage[page].rever();
			menu[tab].rever();
			window.print(page);
			s=0;
			refresh_list=1;
		}
		if (refresh_list) {
			window.p[last+page*24].show();
			int tmp=min((int)window.p.size()-page*24,24);
			s=(s+tmp)%tmp;
			window.p[s+page*24].rever();
		}
	}
	UnHideCursor();
	system("clear");
	ofstream fout;
	fout.open("judgelog");
	char dot=' ';
	for (int i=0;i<oier.size();i++) {
		//if (i)
			//fout << endl;
		fout << oier[i].name_to_print << endl << oier[i].sum << dot;
		for (int j=0;j<oier[i].problem.size();j++) {
			JudgeResult *p=&oier[i].problem[j];
			fout << p->st << dot << p->time << dot << p->memo << dot << p->score << dot;
			for (int k=0;k<p->subresult.size();k++)
				fout << p->subresult[k].st << dot << p->subresult[k].time << dot << p->subresult[k].memo << dot << p->subresult[k].score << dot;
		}
	}
	fout.close();
	return ;
}
