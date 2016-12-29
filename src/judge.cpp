#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Conio.h"
#include "Functions_Linux.h"
#include "judge.h"
#include "config_judge.h"
#include "MultiCompiler.h"
#include "TrieTree.h"

#ifdef EN
#include "lang/language_en.h"
#else
#include "lang/language.h"
#endif

using namespace std;

bool cmp(char *str,int s,char *word) {//比较字符串，从str的第s个和word的第1个字符开始比较
	int pos=0;
	for (int i=0;i<strlen(word);i++) {
		if (s+i+pos>=strlen(str))
			return false;
		while (str[s+i+pos]==' '&&word[i]!=' ')
				pos++;
		if (str[s+i+pos]!=word[i])
			return false;
	}
	return true;
}

bool cmp(string str,int s,char *word) {
	int pos=0;
	for (int i=0;i<strlen(word);i++) {
		if (s+i+pos>=str.length())
			return false;
		int bakpos=pos-1;
		while (pos!=bakpos) {
			bakpos=pos;
			while (str[s+i+pos]==' '&&word[i]!=' ')
				pos++;
			while ((s+i+pos+1<str.length())&&str[s+i+pos]=='#'&&str[s+i+pos+1]=='#')
				pos+=2;
		}
		if (str[s+i+pos]!=word[i])
			return false;
	}
	return true;
}

inline char FirstChar(string str) {
	for (int i=0;i<str.length();i++)
		if ((str[i]!=' ')&&(str[i]!='\t'))
			return str[i];
	return 0;
}

int JudgeSettings::ConverttoInt(string colorname) {
	if (colorname=="black")
		return black;
	else if (colorname=="blue")
		return blue;
	else if (colorname=="green")
		return green;
	else if (colorname=="cyan")
		return cyan;
	else if (colorname=="red")
		return red;
	else if (colorname=="purple")
		return purple;
	else if (colorname=="yellow")
		return yellow;
	else if (colorname=="white")
		return white;
	else
		return black;
}

bool HeadersCheck(string str,int line,string filename) {//检查一行include是否包含非法头文件
	int useown=0;
	string head;
	int flag=0,wrong=0;
	for (int i=0;i<str.length();i++) {
		if (str[i]=='"') {
			if (useown)
				flag=0;
			else {
				useown++;
				flag=1;
				wrong=1;
			}
		}
		if (str[i]=='>')
			flag=0;
		if (flag&&str[i]!=' '&&str[i]!='"') {
			head.push_back(str[i]);
			continue;
		}
		if (str[i]=='<')
			flag=1;
	}
	int k;
	if (wrong||(((k=JudgeSettings::Header.query(head))!=-1)&&(k==head.length()))) {
		PRINTERR:;
		if (JudgeSettings::Terminal) {
			ClearColor();
			HighLight();
		}
		printf("%s:%d: ",filename.c_str(),line);
		JudgeOutput::PrintError();
		printf("%s:%s\n",Context::InvalidHeaderFound,head.c_str());
		return true;
	}
	return false;
}

int SourceProblem_Deal(Contest *x,Problem *p,ifstream &fin,string l) {
	if (l=="time"||l=="t") {
		double t;
		fin >> t;
		p->timelimit=t;
	}
	else if (l=="score"||l=="s") {
		int sc;
		fin >> sc;
		p->eachscore=sc;
		cout << p->name_to_print << ' ' << p->eachscore << endl;
	}
	else if (l=="memo"||l=="memory"||l=="m") {
		int m;
		fin >> m;
		p->memorylimit=m;
	}
	else if (l=="rename"||l=="ren"||l=="r") {
		l.clear();
		char ch;
		while (fin >> ch) {
			if (ch=='\\') {
				fin >> ch;
				l.push_back(ch);
				continue;
			}
			if (ch==')')
				break;
			l.push_back(ch);
		}
		p->name_to_print=l;
	}
	else if (l=="quit")
		return 0;
	else
		cerr << "Unknown command: " << l << endl;
	return 1;
}

void JudgeSettings::SourceProblem(string name,ifstream &fin,Contest *x) {
	string l;
	Problem *p=NULL;
	for (int i=0;i<x->problem.size();i++)
		if (x->problem[i].name=="./data"+name+"/"||x->problem[i].name=="./data/"+name+"/")
			p=&x->problem[i];
	if (p==NULL) {
		cerr << "Error: problem not found: " << name << endl;
		return ;
	}
	while (getline(fin,l,'(')) {
		SourceProblem_Deal(x,p,fin,l);
		getline(fin,l,'\n');
	}
	fin.close();
}

void GetLine(ifstream &fin,string &l,char ch) {
	char tmp;
	l.clear();
	while (fin >> tmp) {
		if (tmp=='\\') {
			fin >> tmp;
			l.push_back(tmp);
			continue;
		}
		if (tmp==ch)
			return ;
		l.push_back(tmp);
	}
}

void JudgeSettings::ReadSettings(const char *settingsfile,Contest *x) {
	ifstream fin;
	fin.open(settingsfile);
	if (!fin)
		return ;
	string l;
	vector<TrieTree*> lis={&JudgeSettings::Func,&JudgeSettings::Header};
	int p;
	Problem *Pro;
	int type=0;//配置全局变量还是单个问题
	while (getline(fin,l,'(')) {
		p=-1;

		if (type==1)
			type=SourceProblem_Deal(x,Pro,fin,l);
		else if (type==0) {
			if (l=="background"||l=="bg") {
				getline(fin,l,')');
				JudgeSettings::Status_Backround=ConverttoInt(l);
			}
			else if (l=="SourceProblem"||l=="source"||l=="s") {
				string pathto;
				GetLine(fin,l,',');//problem name
				GetLine(fin,pathto,')');
				ifstream tmp;
				tmp.open(pathto);
				if (!tmp) {
					cerr << "Error: config file not found: " << pathto << endl;
					continue;
				}
				SourceProblem(l,tmp,x);
			}
			else if (l=="Func"||l=="f")
				p=0;
			else if (l=="Header"||l=="h")
				p=1;
			else if (l=="enter") {
				GetLine(fin,l,')');
				Pro=NULL;
				for (int i=0;i<x->problem.size();i++)
					if (x->problem[i].name_to_print==l)
						Pro=&x->problem[i];
				type=1;
				if (Pro==NULL) {
					cerr << "Error: problem not found: " << l << endl;
					type=0;
				}
			}
			else if (l=="compiler") {
				string a,b,c;
				getline(fin,a,',');
				getline(fin,b,',');
				getline(fin,c,')');
				Compiler.push_back((COMPILER){a,b,c});
			}
			else
				cerr << "Unknown command: " << l << endl;

			if (p!=-1)
				while (true) {
					l.clear();
					char ch;
					while (fin >> ch) {
						if (ch=='\\') {
							fin >> ch;
							l.push_back(ch);
							continue;
						}
						if ((ch==')')||(ch==','))
							break;
						l.push_back(ch);
					}
					if (lis[p]==&JudgeSettings::Func)
						l+='(';
					lis[p]->add(l);
					if (ch==')')
						break;
				}
		}
		
		getline(fin,l,'\n');
	}
	fin.close();
}

int JudgeSettings::ReadFromArgv(int c,char *v[]) {
	for (int i=1;i<c;i++)
		if (v[i][0]=='-') {
			i++;
			if (cmp(v[i-1],0,(char *)"-t")&&(strlen(v[i-1])==2)&&(i<c))//时间
				sscanf(v[i],"%lf",&JudgeSettings::Default_timelimit);
			else if (cmp(v[i-1],0,(char *)"-m")&&(strlen(v[i-1])==2)&&(i<c))//内存
				sscanf(v[i],"%d",&JudgeSettings::Default_memorylimit);
			else if (cmp(v[i-1],0,(char *)"-f")&&(strlen(v[i-1])>=2)&&(i<c)) {//添加禁用函数，如-f3表示添加接下的的三个单词，-f与-f1等效
				int t;
				if (sscanf(v[i-1],"-f%d",&t)==-1)
					t=1;
				for (int k=0;(k<t)&&(i<c);k++) {
					char str[256];
					sprintf(str,"%s",v[i]),i++;
					string tmp=(string)str+"(";
					JudgeSettings::Func.add(tmp);
				}
				i--;
			}
			else if (cmp(v[i-1],0,(char *)"-h")&&(strlen(v[i-1])>=2)&&(i<c)) {//添加禁用头文件，如-h3表示添加接下的的三个头文件，-h与-h1等效
				int t;
				if (sscanf(v[i-1],"-h%d",&t)==-1)
					t=1;
				for (int k=0;(k<t)&&(i<c);k++) {
					char str[256];
					sprintf(str,"%s",v[i]),i++;
					string tmp=str;
					JudgeSettings::Header.add(tmp);
				}
				i--;
			}
			else if ((string)v[i-1]==(string)"--csv")
				JudgeSettings::PrinttoCSV=1;
			else if ((string)v[i-1]==(string)"--nocsv")
				JudgeSettings::PrinttoCSV=0;
			else if (((string)v[i-1]==(string)"-c")||((string)v[i-1]==(string)"--cui"))
				JudgeSettings::UseCUI=1;
			else if ((cmp(v[i-1],0,(char *)"--help")&&(strlen(v[i-1])==6))) {
				Context::PrintHelp();
				return 1;
			}
			else{
				Context::WrongArgument(v[i-1]);
				return 1;
			}
		}
	return 0;
}

void ReadLine(FILE *fp,string &line) {
	line.clear();
	char ch;
	while (fscanf(fp,"%c",&ch)==1) {
		if (ch=='\n')
			break;
		line.push_back(ch);
		if (line.length()>50)
			line.erase(0,1);
	}
}

JudgeResult RunAnalyse(string filename,int memorylimit) {
//解析输出。
//当程序超时，timeout终结进程时，输出如下：
//	  Command terminated by signal 9
//	  Time:1.00s Memory:1732KB
//当程序运行错误时，输出如下：
//	  timeout: the monitored command dumped core
//	  Command terminated by signal 11
//	  Time:0.02s Memory:1924KB
//或者：
//    Command exited with non-zero status 1
//    Time:0.00s Memory:1988KB
	system(("tac "+filename+" > .ejudge.analyse").c_str());//从最后一行开始解析，防止程序输出到stderr
	JudgeResult res;
	res.score=0;
	string line;
	FILE *fp=fopen(".ejudge.analyse","r");
	ReadLine(fp,line);
	int k=0;
	for (int i=line.size()-1;i>=0;i--)
		if (line[i]=='T') {
			k=i;
			break;
		}
	char tmpstr[256];
	sprintf(tmpstr,"%s",line.substr(k,line.size()-k).c_str());
	sscanf(tmpstr,"Time:%lfs Memory:%dKB\n",&res.time,&res.memo);
	if (res.memo==0) {
		res.st=RE;
		return res;
	}
	if (res.memo>memorylimit) {
		res.st=MLE;
		return res;
	}
	ReadLine(fp,line);
	if (line=="") {
		res.st=-2333;
		return res;
	}
	string TLE_RE="Command terminated by signal";
	string RE_top="timeout: the monitored command dumped core";
	string RE_1="Command exited with non-zero status";
	for (int i=0;i<line.size();i++) {
		if ((line.size()-i>=TLE_RE.length())&&(line.substr(i,TLE_RE.length())==TLE_RE)) {
			string line2;
			ReadLine(fp,line2);
			if (line2=="") {
				res.st=TLE;
				goto END;
			} else {
				res.st=RE;
				goto END;
			}
		}
		if ((line.size()-i>=RE_1.length())&&(line.substr(i,RE_1.length())==RE_1)) {
			res.st=RE;
			goto END;
		}
	}
	END:;
	fclose(fp);
	return res;
}

bool TestPoint::operator<(TestPoint x) {
	if (stdInput.length()!=x.stdInput.length())
		return stdInput.length()<x.stdInput.length();
	for (int i=0;i<stdInput.length();i++)
		if (stdInput[i]!=x.stdInput[i])
			return stdInput[i]<x.stdInput[i];
}

JudgeResult TestPoint::JudgePoint(string bin,double timelimit,int memorylimit,int MaxScore,string &Directory) {
	int RunAsRoot=WEXITSTATUS(system("if [[ $EUID -eq 0 ]]; then exit 1;fi"));
	char str[1024];
	if (JudgeSettings::use_freopen) {
		if (RunAsRoot) {
			system(("cp "+stdInput+" Exec/"+bin+".in").c_str());
			sprintf(str,"time -f \"Time:%%es Memory:%%MKB\" timeout --signal=KILL %lfs chroot Exec %s / 2>.ejudge.run > /dev/null",timelimit,bin.c_str());
		} else {
			system(("cp "+stdInput+" "+bin+".in").c_str());
			sprintf(str,"time -f \"Time:%%es Memory:%%MKB\" timeout --signal=KILL %lfs ./Exec/%s 2>.ejudge.run >/dev/null",timelimit,bin.c_str());
		}
		system(str);
		if (RunAsRoot) {
			system(("rm Exec/"+bin+".in").c_str());
			system(("mv Exec/"+bin+".out ./.ejudge.tmp 2>/dev/null").c_str());
		} else {
			system(("rm "+bin+".in").c_str());
			system(("mv "+bin+".out .ejudge.tmp 2>/dev/null").c_str());
		}
	} else {
		if (WEXITSTATUS(system("if [[ $EUID -eq 0 ]]; then exit 1;fi")))
			sprintf(str,"time -f \"Time:%%es Memory:%%MKB\" timeout --signal=KILL %lfs chroot Exec %s / < %s > .ejudge.tmp 2>.ejudge.run",timelimit,bin.c_str(),stdInput.c_str());
		else
			sprintf(str,"time -f \"Time:%%es Memory:%%MKB\" timeout --signal=KILL %lfs ./Exec/%s < %s > .ejudge.tmp 2>.ejudge.run",timelimit,bin.c_str(),stdInput.c_str());//为time命令指定格式获取用时和内存使用，并用timeout命令限制运行时间。
		system(str);
	}
	JudgeResult res=RunAnalyse(".ejudge.run",memorylimit);
	if (res.st!=-2333)
		return res;
	if (exist(Directory+"spj")) {//SPJ support
 //		- argv[1]: 标准输入文件
 //		- argv[2]: 选手输出文件
 //		- argv[3]: 标准输出文件
 //		- argv[4]: 本测试点满分
 //		- argv[5]: 分数输出文件(必须创建),仅一行,包含一个非负整数,表示得分.
 //		- argv[6]: 额外信息文件(可以不创建)
		sprintf(str,"%s %s .ejudge.tmp %s %d .ejudge.spj .ejudge.msg",(Directory+"spj").c_str(),stdInput.c_str(),stdOutput.c_str(),MaxScore);
		system(str);
		ifstream fin;
		int score;
		string extrainfo;
		fin.open(".ejudge.spj");
		if (fin) fin >> score;
		fin.close();
		fin.open(".ejudge.msg");
		if (fin) getline(fin,extrainfo,'\n');
		fin.close();
		system("rm .ejudge.tmp");
		return (JudgeResult){((score==MaxScore)?(AC):(WA)),res.memo,res.time,score,(vector<JudgeResult>){},extrainfo};
	}
	sprintf(str,"timeout 1s diff -b -B -Z .ejudge.tmp %s > /dev/null 2>/dev/null && rm .ejudge.tmp",stdOutput.c_str());//比较输出，忽略由空格数不同造成的差异，忽略任何因空行而造成的差异，忽略每行末端的空格。更多用法用法参见diff --help，此设置应在大多数情况下有效。
	if (WEXITSTATUS(system(str))!=0)
		return (JudgeResult){WA,res.memo,res.time,0};
	else
		return (JudgeResult){AC,res.memo,res.time,MaxScore};
}

void Contestant::sumup() {
	sum=0;
	for (int i=0;i<problem.size();i++)
		sum+=problem[i].score;
}

bool Contestant::operator<(Contestant &x) {
	return sum>x.sum;
}

bool Problem::SafetyCheck(string filename) {
	JudgeSettings::use_freopen=0;
	ifstream fin;
	fin.open(filename);
	if (!fin) {
		if (JudgeSettings::Terminal) {
			ClearColor();
			HighLight();
		}
		cout << filename << ": ";
		JudgeOutput::PrintError();
		puts(Context::SourceNotFound);
		return true;
	}
	if (!((filename.length()>=3&&cmp(filename.c_str(),filename.length()-3,(char *)"cpp")||(filename.length()>=1&&filename[filename.length()-1]=='c'))))
		return false;
	int extraline=0;
	{//预处理宏
		const string inc="#include";
		ifstream fin;
		ofstream fout;
		fin.open(filename);
		if (!fin)
			return 0;
		fout.open(".ejudge.tmp.cpp");
		string l;
		int linenumber=0;
		while (getline(fin,l,'\n')) {
			int mark=0;
			for (int i=0;i<l.length();i++)
				if (l[i]=='#') {
					extraline++;
					if (l.length()-i>=inc.length()) {
						if (l.substr(i,inc.length())!=inc)
							continue;
						linenumber++;
						string line=l;
						while (l[l.length()-1]=='\\') {
							line.pop_back();
							getline(fin,l,'\n');
							for (int w=0;w<l.length();w++)
								line.push_back(l[w]);
							extraline++;
							linenumber++;
						}
						if (HeadersCheck(line,linenumber,filename))
							return true;
						goto END;
						}
				} else if (l[i]!=' '&&l[i]!='\t')
					break;
			linenumber++;
			fout << l << endl;
			END:;
		}
		fin.close();
		fout.close();
		system("g++ -E -P -DEJUDGE .ejudge.tmp.cpp > .ejudge.cpp");
	}
	fin.close();
	fin.open(".ejudge.cpp");
	string str;
	int line=0;
	while (getline(fin,str,'\n')) {
		string tmp;
		while (str[str.length()-1]=='\\') {
			str.pop_back();
			getline(fin,tmp,'\n');
			for (int w=0;w<tmp.length();w++)
				str.push_back(tmp[w]);
		}
		line++;
		for (int i=0;i<str.length();i++)
			if (cmp(str,i,(char *)"freopen("))
				JudgeSettings::use_freopen=1;
			else {
				int k;
				if ((k=JudgeSettings::Func.query(str,i))!=-1) {
					if (JudgeSettings::Terminal) {
						ClearColor();
						HighLight();
					}
					printf("%s:%d: ",filename.c_str(),line+extraline);
					JudgeOutput::PrintError();
					printf("%s:%s\n",Context::InvalidWordFound,(str.substr(i,k-i-1)).c_str());
					return true;
				}
				if ((k=JudgeSettings::Const.query(str,i))!=-1) {
					if (JudgeSettings::Terminal) {
						ClearColor();
						HighLight();
					}
					printf("%s:%d: ",filename.c_str(),line+extraline);
					JudgeOutput::PrintError();
					printf("%s:%s\n",Context::InvalidConstFound,(str.substr(i,k-i)).c_str());
					return true;
				}
			}
	}
	fin.close();
	return false;
}

void Problem::InitProblem() {
	if (exist(name+(string)".in")) {
		system(((string)"mv "+name+(string)".in"+" "+name+(string)"_.in").c_str());
		system(((string)"mv "+name+(string)".out"+" "+name+(string)"_.out").c_str());
	}
	vector<string> filelist=GetFile(name.c_str(),".in");
	for (int i=0;i<filelist.size();i++) {
		TestPoint tmp;
		tmp.stdInput=name+filelist[i];
		tmp.stdOutput=name+filelist[i].replace(filelist[i].length()-3,3,".out");
		if (!exist(tmp.stdOutput))
			continue;
		point.push_back(tmp);
	}
	memorylimit=JudgeSettings::Default_memorylimit;
	timelimit=JudgeSettings::Default_timelimit;
	sort(point.begin(),point.end());
	if (point.size()&&eachscore==0)
		eachscore=100/point.size();
}

JudgeResult Problem::JudgeProblem(Contestant &oier){
	JudgeResult tot,tmpresult;
	tot.score=0;
	tot.st=AC;
	tot.memo=-1;
	tot.time=-1;
	putchar('\n');
	if (Compile(oier.name,name_to_print,JudgeSettings::Terminal)) {
		if (!JudgeSettings::Terminal) {
			fstream fin;
			fin.open(".ejudge.tmp");
			string line;
			while (getline(fin,line,'\n'))
				cout << line << endl;
			fin.close();
		}
		vector<JudgeResult> sub;
		for (int i=0;i<point.size();i++)
			sub.push_back((JudgeResult){CE,0,0,0});
		return (JudgeResult){CE,0,0,0,sub};
	}
	int maxlength=-(1<<30);
	for (int i=0;i<point.size();i++)
		maxlength=max(maxlength,(int)BaseName(point[i].stdInput).length());
	char tmp[256];
	sprintf(tmp,"%%%ds ",maxlength+3);
	for (int i=0;i<point.size();i++) {
		printf(tmp,BaseName(point[i].stdInput).c_str());
		JudgeOutput::PrintResult(tmpresult=(point[i].JudgePoint("./"+name_to_print,timelimit,memorylimit,eachscore,name)));
		tot.score+=tmpresult.score;
		if ((tot.st==AC)&&(tmpresult.st!=AC))
			tot.st=tmpresult.st;
		tot.subresult.push_back(tmpresult);
		tot.memo=max(tot.memo,tmpresult.memo);
		tot.time=max(tot.time,tmpresult.time);
	}
	system(("rm ./Exec/"+name_to_print).c_str());
	return tot;
}

void Contest::InitSPJ() {
	for (int i=0;i<problem.size();i++)
		if ((exist(problem[i].name+"spj.cpp"))&&(!exist(problem[i].name+"spj")))
			system(("g++ "+problem[i].name+"spj.cpp -o "+problem[i].name+"spj -O2 2>/dev/null").c_str());
}

void Contest::InitContest() {
	if (!PrinttoTerminal(JudgeSettings::PrintDevice))
		JudgeSettings::Terminal=0;
	if (JudgeSettings::UseCUI&&exist("judgelog")) {
		Config::Readfrom(this,"judgelog");
		return ;
	}
	vector<string> filelist=GetFile(".",".cpp");
	vector<string> tmp;
	for (int i=0;i<Compiler.size();i++) {
		tmp=GetFile(".",("."+Compiler[i].suffix).c_str());
		for (int j=0;j<tmp.size();j++)
			filelist.push_back(tmp[j]);
	}
	if (filelist.size()) {
		Contestant user;
		user.name="./";
		user.name_to_print=GetUserName();
		oier.push_back(user);
	}
	filelist=GetDir("./source");
	for (int i=0;i<filelist.size();i++) {
		Contestant tmp;
		tmp.name="./source/"+filelist[i]+"/";
		tmp.name_to_print=filelist[i];
		oier.push_back(tmp);
	}

	filelist=GetFile(".",".in");
	if (filelist.size()) {
		Problem Default;
		Default.name="./";
		Default.name_to_print=GetName();
		Default.InitProblem();
		if (Default.point.size())
			problem.push_back(Default);
	}
	filelist=GetDir("./data");
	for (int i=0;i<filelist.size();i++) {
		Problem tmp;
		tmp.name="./data/"+filelist[i]+"/";
		tmp.name_to_print=filelist[i];
		tmp.InitProblem();
		if (tmp.point.size())
			problem.push_back(tmp);
	}

	if (JudgeSettings::PrinttoCSV==-1) {
		if (problem.size()>1)
			JudgeSettings::PrinttoCSV=1;
		else
			JudgeSettings::PrinttoCSV=0;
	}
}

void Contest::JudgeContest() {
	if (JudgeSettings::UseCUI&&exist("judgelog")) {
		Judge_CUI();
		return ;
	}

	for (int i=0;i<oier.size();i++) {
		for (int j=0;j<problem.size();j++) {
			if (JudgeSettings::Terminal)
				foreground(yellow);
			else
				cout << Context::Contestant+" " << i+1 << ':' << oier[i].name_to_print << " "+Context::Problem+" " << j+1 << ':' << problem[j].name_to_print << " [" << i*problem.size()+j+1 << '/' << oier.size()*problem.size() << ']' << endl;
			cerr << Context::Contestant+" " << i+1 << ':' << oier[i].name_to_print << " "+Context::Problem+" " << j+1 << ':' << problem[j].name_to_print << " [" << i*problem.size()+j+1 << '/' << oier.size()*problem.size() << ']' << endl;
			if (JudgeSettings::Terminal)
				ClearColor();
			while (oier[i].problem.size()<=j)
				oier[i].problem.push_back((JudgeResult){});
			oier[i].problem[j]=problem[j].JudgeProblem(oier[i]);
			printf("\n%s ",oier[i].name_to_print.c_str());
			JudgeOutput::PrintResult(oier[i].problem[j]);
			printf("%s:%d\n\n",Context::Score,oier[i].problem[j].score);
		}
		oier[i].sumup();
	}

	Config::Saveto(this,"judgelog");
	if (JudgeSettings::UseCUI)
		Contest::Judge_CUI();
}

void JudgeOutput::PrintStatus(int st) {
	if (JudgeSettings::Terminal) {	
		color(JudgeSettings::Status_Color[st],JudgeSettings::Status_Backround);
		HighLight();
		printf("%s",JudgeSettings::Status[st]);
		ClearColor();
	}
	else
		printf("%s",JudgeSettings::Status[st]);
}

void JudgeOutput::PrintError() {
	if (JudgeSettings::Terminal) {
		foreground(red);
		HighLight();
		cout << Context::Error;
		ClearColor();
	}
	else
		cout << Context::Error;
}

void JudgeOutput::PrintResult(JudgeResult &x) {
	if (JudgeSettings::Terminal) {
		foreground(cyan);
		printf("%s:",Context::Time);
		foreground(yellow);
		printf("%5.2lfs",x.time);
		foreground(cyan);
		printf(" %s:",Context::Memory);
		foreground(yellow);
		printf("%7dKB ",x.memo);
		PrintStatus(x.st);
		ClearColor();
		cout << ' ' << x.ExtraInfo << endl;
		ClearColor();
	}
	else {
		printf("%s:%5.2lfs %s:%7dKB ",Context::Time,x.time,Context::Memory,x.memo);
		PrintStatus(x.st);
		cout << ' ' << x.ExtraInfo << endl;
	}
}

void JudgeOutput::Print_zh_CN(string str,int len) {//修复输出问题
	int l=0;
	for (int i=0;i<str.length();i++)
		if (str[i]<0)
			l++;
	l/=3;
	char tmp[512];
	if (l==0) {
		sprintf(tmp,"%%%ds",len);
		printf(tmp,str.c_str());
	}
	else if (l==2) {
		sprintf(tmp,"%c%c%c  %c%c%c",str[0],str[1],str[2],str[3],str[4],str[5]);
		char tmp2[512];
		sprintf(tmp2,"%%%ds",len+2);
		printf(tmp2,tmp);
	}
	else {
		sprintf(tmp,"%%%ds",len+l);
		printf(tmp,str.c_str());
	}
}

void JudgeOutput::OutputContest(Contest &test) {
	if (JudgeSettings::UseCUI||(test.oier.size()<=1&&test.problem.size()<=1))
		return ;
	sort(test.oier.begin(),test.oier.end());
	int len=6;
	for (int i=0;i<test.oier.size();i++)
		len=max(len,(int)test.oier[i].name_to_print.length());
	for (int i=0;i<len+1;i++)
		putchar(' ');
	Print_zh_CN(Context::TotalScore.c_str(),11);
	for (int i=0;i<test.problem.size();i++)
		printf("%11s",test.problem[i].name_to_print.c_str());
	puts("");
	for (int i=0;i<test.oier.size();i++) {
		JudgeOutput::Print_zh_CN(test.oier[i].name_to_print,len);
		putchar(':');
		printf("%11d",test.oier[i].sum);
		for (int j=0;j<test.problem.size();j++)
			printf("%11d",test.oier[i].problem[j].score);
		puts("");
	}
	if (JudgeSettings::PrinttoCSV)
		ConverttoCSV(test,(string)"result.csv");
}

void JudgeOutput::ConverttoCSV(Contest &test,string csv) {
	ofstream fout;
	fout.open(csv.c_str());
	sort(test.oier.begin(),test.oier.end());
	fout << ",\"" << Context::TotalScore << "\",";
	for (int i=0;i<test.problem.size();i++)
		fout << "\"" << test.problem[i].name_to_print << "\",,";
	fout << endl;
	for (int i=0;i<test.oier.size();i++) {
		fout << "\"" << test.oier[i].name_to_print << "\"," << test.oier[i].sum << ",";
		for (int j=0;j<test.problem.size();j++) {
			for (int k=0;k<test.oier[i].problem[j].subresult.size();k++)
				fout << JudgeSettings::Status_Short[test.oier[i].problem[j].subresult[k].st];
			fout << ',' << test.oier[i].problem[j].score << ',';
		}
		fout << endl;
	}
	fout << ',' << endl;
	for (int i=0;i<3+test.problem.size()*2;i++)
		fout << ',';
	fout << "\"" << Context::SymbolExplanation << endl;
	for (int i=0;i<6;i++)
		fout << JudgeSettings::Status_Short[i] << ":" << JudgeSettings::Status[i] << endl;
	fout << "\"," << endl;
}
