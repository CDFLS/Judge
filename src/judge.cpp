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
		int bakpos=pos-1;
		while (pos!=bakpos) {
			bakpos=pos;
			while (str[s+i+pos]==' '&&word[i]!=' ')
				pos++;
			while ((s+i+pos+1<strlen(str))&&str[s+i+pos]=='#'&&str[s+i+pos+1]=='#')
				pos+=2;
		}
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
	char head[256],l=0;
	int flag=0;
	for (int i=0;i<str.length();i++) {
		if (str[i]=='>')
			flag=0;
		if (flag&&str[i]!=' ') {
			head[l++]=str[i];
			continue;
		}
		if (str[i]=='<')
			flag=1;
	}
	head[l]=0;
	for (int i=0;i<JudgeSettings::InvalidHeaders.size();i++)
		if (cmp((char *)JudgeSettings::InvalidHeaders[i].c_str(),0,head)&&(JudgeSettings::InvalidHeaders[i].length())==strlen(head)) {
			if (JudgeSettings::Terminal) {
				ClearColor();
				HighLight();
			}
			printf("%s:%d: ",filename.c_str(),line);
			JudgeOutput::PrintError();
			printf("%s:%s\n",Context::InvalidHeaderFound,head);
			return true;
		}
	return false;
}

void JudgeSettings::SourceProblem(string name,string source,Contest *x) {
	ifstream fin;
	string bak=source;
	fin.open(source);
	if (!fin) {
		cerr << "Error: config file not found: "+source << endl;
		return ;
	}
	string l;
	Problem *p=NULL;
	for (int i=0;i<x->problem.size();i++)
		if (x->problem[i].name=="./data"+name+"/"||x->problem[i].name=="./data/"+name+"/")
			p=&x->problem[i];
	if (p==NULL) {
		cerr << "Error: problem not found: " << name << endl;
		return ;
	}
	while (getline(fin,l,'=')) {
		if (l=="time"||l=="t") {
			double t;
			fin >> t;
			p->timelimit=t;
		}
		else if (l=="score"||l=="s") {
			int sc;
			fin >> sc;
			p->eachscore=sc;
		}
		else if (l=="memo"||l=="memory"||l=="m") {
			int m;
			fin >> m;
			p->memorylimit=m;
		}
		getline(fin,l,'\n');
	}
	fin.close();
}

void JudgeSettings::ReadSettings(const char *settingsfile,Contest *x) {
	ifstream fin;
	fin.open(settingsfile);
	if (!fin)
		return ;
	string l;
	int InvalidFuncNumber=1,InvalidHeadersNumber=1;
	while (getline(fin,l,'=')) {
		if (l=="background"||l=="bg") {
			fin >> l;
			JudgeSettings::Status_Backround=ConverttoInt(l);
		}
		else if (l=="InvalidFunc"||l=="if") {
			for (int i=0;i<InvalidFuncNumber;i++) {
				fin >> l;
				JudgeSettings::InvalidFunc.push_back(l);
			}
		}
		else if (l=="InvalidFuncNumber"||l=="ifn") {
			fin >> InvalidFuncNumber;
		}
		else if (l=="InvalidHeaders"||l=="ih") {
			for (int i=0;i<InvalidHeadersNumber;i++) {
				fin >> l;
				JudgeSettings::InvalidHeaders.push_back(l);
			}
		}
		else if (l=="InvalidHeadersNumber"||l=="ihn") {
			fin >> InvalidHeadersNumber;
		}
		else if (l=="SourceProblem"||l=="source"||l=="sp") {
			string pathto;
			getline(fin,l,'|');//problem name
			fin >> pathto;
			SourceProblem(l,pathto,x);
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
					string tmp=str;
					JudgeSettings::InvalidFunc.push_back(tmp);
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
					JudgeSettings::InvalidHeaders.push_back(tmp);
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

JudgeResult TestPoint::JudgePoint(string bin,double timelimit,int memorylimit,int MaxScore,string &Directory) {
	int s=AC,memo,RunAsRoot=WEXITSTATUS(system("if [[ $EUID -eq 0 ]]; then exit 1;fi"));
	double time;
	char str[1024];
	if (JudgeSettings::use_freopen) {
		if (RunAsRoot) {
			system(("cp "+stdInput+" Exec/"+bin+".in").c_str());
			sprintf(str,"time -f \"Time:%%es Memory:%%MKB\" timeout --signal=KILL %lfs chroot Exec %s / 2>.ejudge.run",timelimit,bin.c_str());
		} else {
			system(("cp "+stdInput+" "+bin+".in").c_str());
			sprintf(str,"time -f \"Time:%%es Memory:%%MKB\" timeout --signal=KILL %lfs ./Exec/%s 2>.ejudge.run",timelimit,bin.c_str());
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
	FILE *fp=fopen(".ejudge.run","r");
	char ch;
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
//故有如下解析代码：
	fscanf(fp,"%c",&ch);
	if (ch=='C') {
		for (int i=0;i<8;i++)
			fscanf(fp,"%c",&ch);
		if (ch=='e')
			s=RE;
		else
			s=TLE;
		while (ch!='T')
			fscanf(fp,"%c",&ch);
	} else if (ch=='t') {
		s=RE;
		while (ch!='T')
			fscanf(fp,"%c",&ch);
	}
	fscanf(fp,"ime:%lfs Memory:%dKB",&time,&memo);
	fclose(fp);
	if (s==TLE)
		return (JudgeResult){TLE,memo,time,0};
	if (s==RE)
		return (JudgeResult){RE,0,0,0};
	if (memo>memorylimit)
		return (JudgeResult){MLE,memo,time,0};
	if (memo==0)
		return (JudgeResult){RE,memo,time,0};
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
		if (fin) fin >> extrainfo;
		fin.close();
		system("rm .ejudge.tmp");
		return (JudgeResult){((score==MaxScore)?(AC):(WA)),memo,time,score,(vector<JudgeResult>){},extrainfo};
	}
	sprintf(str,"timeout 1s diff -b -B -Z .ejudge.tmp %s > /dev/null 2>/dev/null && rm .ejudge.tmp",stdOutput.c_str());//比较输出，忽略由空格数不同造成的差异，忽略任何因空行而造成的差异，忽略每行末端的空格。更多用法用法参见diff --help，此设置应在大多数情况下有效。
	if (WEXITSTATUS(system(str))!=0)
		return (JudgeResult){WA,memo,time,0};
	else
		return (JudgeResult){AC,memo,time,MaxScore};
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
		JudgeOutput::PrintError();
		puts(Context::SourceNotFound);
		return true;
	}
	if (!((filename.length()>=3&&cmp(filename.c_str(),filename.length()-3,(char *)"cpp")||(filename.length()>=1&&filename[filename.length()-1]=='c'))))
		return false;
	string str;
	int line=0,ifndef=0,flag=0,ifdef=0;//忽略在注释和#ifndef EJUDGE和#ifdef EJUDGE #else中的单词
	while (getline(fin,str,'\n')) {
		line++;
		int include=0,First=1;
		for (int i=0;i<str.length();i++)
			if ((str[i]=='/')&&(str[i+1]=='/'))
				break;
			else if ((str[i]=='*')&&(str[i+1]=='/'))
				flag=0;
			else if ((str[i]=='/')&&(str[i+1]=='*'))
				flag=1;
			else if (cmp(str,i,(char *)"#ifndef EJUDGE"))
				ifndef=1;
			else if (cmp(str,i,(char *)"#ifdef EJUDGE"))
				ifdef=1;
			else if (cmp(str,i,(char *)"#endif"))
				ifndef=0,ifdef=0;
			else if (cmp(str,i,(char *)"#else")) {
				ifndef=0;
				if (ifdef==1)
					ifdef=2;
			}
			else if (cmp(str,i,(char *)"#define")&&FirstChar(str)=='#') {
				int w=i,cnt=0;
				for (;w<str.length();w++) {
					if (str[w]==' ')
						cnt++;
					if (cnt==2)
						break;
				}
				w++;
				for (int k=0;k<JudgeSettings::InvalidFunc.size();k++)
					if (cmp(str,w,(char *)JudgeSettings::InvalidFunc[k].c_str())) {
						if (JudgeSettings::Terminal) {
							ClearColor();
							HighLight();
						}
						printf("%s:%d: ",filename.c_str(),line);
						JudgeOutput::PrintError();
						printf("%s:%s\n",Context::InvalidWordFound,JudgeSettings::InvalidFunc[k].c_str());
						return true;
					}
			}
			else if (flag||ifndef||(ifdef==2))
				continue;
			else if (cmp(str,i,(char *)"#include")&&FirstChar(str)=='#') {
				if (HeadersCheck(str,line,filename))
					return true;
				include=1;
			}
			else if (str[i]=='"'&&include&&First) {//不允许调用自定义头文件
				if (JudgeSettings::Terminal) {
					ClearColor();
					HighLight();
				}
				printf("%s:%d: ",filename.c_str(),line);
				JudgeOutput::PrintError();
				printf("%s:%s\n",Context::InvalidHeaderFound,str);
				First=0;
				return true;
			}
			else if (cmp(str,i,(char *)"freopen("))
				JudgeSettings::use_freopen=1;
			else {
				for (int k=0;k<JudgeSettings::InvalidFunc.size();k++)
					if (cmp(str,i,(char *)(JudgeSettings::InvalidFunc[k]+"(").c_str())) {
						if (JudgeSettings::Terminal) {
							ClearColor();
							HighLight();
						}
						printf("%s:%d: ",filename.c_str(),line);
						JudgeOutput::PrintError();
						printf("%s:%s\n",Context::InvalidWordFound,JudgeSettings::InvalidFunc[k].c_str());
						return true;
					}
				for (int k=0;k<JudgeSettings::InvalidConst.size();k++)
					if (cmp(str,i,(char *)(JudgeSettings::InvalidConst[k]).c_str())) {
						if (JudgeSettings::Terminal) {
							ClearColor();
							HighLight();
							printf("%s:%d: ",filename.c_str(),line);
							JudgeOutput::PrintError();
							printf("%s:%s\n",Context::InvalidConstFound,JudgeSettings::InvalidConst[k].c_str());
							return true;
						}
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
	vector<string> filelist=GetFile(name,(string)"\\.in");
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
	if (point.size())
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
		if ((int)point[i].stdInput.size()>maxlength)
			maxlength=point[i].stdInput.size();
	char tmp[256];
	sprintf(tmp,"%%%ds ",maxlength+3);
	for (int i=0;i<point.size();i++) {
		printf(tmp,point[i].stdInput.c_str());
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
	vector<string> filelist=GetFile(".","\\.cpp");
	vector<string> tmp;
	for (int i=0;i<SupportedFile.size();i++) {
		tmp=GetFile(".","\\."+SupportedFile[i]);
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

	filelist=GetFile(".","\\.in");
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
	if (JudgeSettings::UseCUI||test.oier.size()<=1)
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
