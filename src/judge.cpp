#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Conio.h"
#include "Functions_Linux.h"
#include "judge.h"

#ifdef EN
#include "lang/language_en.h"
#else
#include "lang/language.h"
#endif

using namespace std;

bool cmp(char *str,int s,string word) {//比较字符串，从str的第s个和word的第1个字符开始比较
	if (strlen(str)-s<word.length())
		return false;
	for (int i=0;i<word.length();i++)
		if (str[s+i]!=word[i])
			return false;
	return true;
}

bool cmp(char *str,int s,char *word) {
	if (strlen(str)-s<strlen(word))
		return false;
	for (int i=0;i<strlen(word);i++)
		if (str[s+i]!=word[i])
			return false;
	return true;
}

bool cmp(string str,int s,char *word) {
	if (str.length()-s<strlen(word))
		return false;
	for (int i=0;i<strlen(word);i++)
		if (str[s+i]!=word[i])
			return false;
	return true;
}

inline char FirstChar(char *str) {
	for (int i=0;i<strlen(str);i++)
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

bool HeadersCheck(char *str,int line,string filename) {//检查一行include是否包含非法头文件
	//for (int i=0;i<JudgeSettings::InvalidHeaders.size();i++)
		//cout << JudgeSettings::InvalidHeaders[i] << endl;
	char head[256],l=0;
	int flag=0;
	for (int i=0;i<strlen(str);i++) {
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
		if (cmp(JudgeSettings::InvalidHeaders[i],0,head)&&(JudgeSettings::InvalidHeaders[i].length())==strlen(head)) {
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

void JudgeSettings::ReadSettings(const char *settingsfile) {
	ifstream fin;
	fin.open(settingsfile);
	string l;
	int InvalidWordsNumber=1,InvalidHeadersNumber=1;
	while (getline(fin,l,'=')) {
		if (l=="background"||l=="bg") {
			fin >> l;
			JudgeSettings::Status_Backround=ConverttoInt(l);
		}
		else if (l=="InvalidWords"||l=="iw") {
			for (int i=0;i<InvalidWordsNumber;i++) {
				fin >> l;
				JudgeSettings::InvalidWords.push_back(l);
			}
		}
		else if (l=="InvalidWordsNumber"||l=="iwn") {
			fin >> InvalidWordsNumber;
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
			else if (cmp(v[i-1],0,(char *)"-w")&&(strlen(v[i-1])>=2)&&(i<c)) {//添加禁用单词，如-w3表示添加接下的的三个单词，-w与-w1等效
				int t;
				if (sscanf(v[i-1],"-w%d",&t)==-1)
					t=1;
				for (int k=0;(k<t)&&(i<c);k++) {
					char str[256];
					sprintf(str,"%s",v[i]),i++;
					string tmp=str;
					JudgeSettings::InvalidWords.push_back(tmp);
				}
				i--;
			}
			else if (cmp(v[i-1],0,(char *)"-s")&&(strlen(v[i-1])>=2)&&(i<c)) {//添加禁用头文件，如-s3表示添加接下的的三个头文件，-s与-s1等效
				int t;
				if (sscanf(v[i-1],"-s%d",&t)==-1)
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
			else if ((cmp(v[i-1],0,(char *)"-h")&&(strlen(v[i-1])==2))||(cmp(v[i-1],0,(char *)"--help")&&(strlen(v[i-1])==6))) {
				cout << Context::help ;
				return 1;
			}
			else{
				Context::WrongArgument(v[i-1]);
				return 1;
			}
		}
	return 0;
}

JudgeResult TestPoint::JudgePoint(string bin,double timelimit,int memorylimit) {
	int s=AC,memo;
	double time;
	char str[512];
	if (JudgeSettings::use_freopen) {
		sprintf(str,"cp %s %s.in",stdInput.c_str(),bin.c_str());
		system(str);
		sprintf(str,"time -f \"Time:%%es Memory:%%MKB\" timeout --signal=KILL %lfs ./%s 2>.ejudge.run",timelimit,bin.c_str());
		system(str);
		sprintf(str,"mv %s.out .ejudge.tmp 2>/dev/null",bin.c_str());
		system(str);
		sprintf(str,"rm %s.in",bin.c_str());
		system(str);
	} else {
		sprintf(str,"time -f \"Time:%%es Memory:%%MKB\" timeout --signal=KILL %lfs %s < %s > .ejudge.tmp 2>.ejudge.run",timelimit,bin.c_str(),stdInput.c_str());//为time命令指定格式获取用时和内存使用，并用timeout命令限制运行时间。
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
//故有如下解析代码：
	fscanf(fp,"%c",&ch);
	if (ch=='C') {
		s=TLE;
		while (ch!='T')
			fscanf(fp,"%c",&ch);
	}
	if (ch=='t') {
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
	sprintf(str,"timeout 1s diff -b -B -Z .ejudge.tmp %s > /dev/null 2>/dev/null",stdOutput.c_str());//比较输出，忽略由空格数不同造成的差异，忽略任何因空行而造成的差异，忽略每行末端的空格。更多用法用法参见diff --help，此设置应在大多数情况下有效。
	if (WEXITSTATUS(system(str))!=0)
		return (JudgeResult){WA,memo,time,0};
	else
		return (JudgeResult){AC,memo,time,MaxScore};
}

JudgeResult TestPoint::JudgePointSPJ(string bin,string spj,double timelimit,int memorylimit) {
	return (JudgeResult){0,0,0,0};
}

void Contestant::sumup() {
	sum=0;
	for (int i=0;i<problem.size();i++)
		sum+=problem[i].score;
}

bool Contestant::operator<(Contestant x) {
	return sum>x.sum;
}

bool Problem::SafetyCheck(string filename) {
	int r=false;
	JudgeSettings::use_freopen=0;
	FILE *fp=fopen(filename.c_str(),"r");
	if (fp==NULL) {
		JudgeOutput::PrintError();
		puts(Context::SourceNotFound);
		return true;
	}
	char str[1024];
	int line=0,ifndef=0,flag=0,ifdef=0;//忽略在注释和#ifndef EJUDGE和#ifdef EJUDGE #else中的单词
	while (fgets(str,2000,fp)!=NULL) {
		if (str[strlen(str)-1]=='\n')
			str[strlen(str)-1]=0;
		line++;
		int include=0,First=1;
		for (int i=0;i<strlen(str);i++)
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
			else if (flag||ifndef||(ifdef==2))
				continue;
			else if (cmp(str,i,(char *)"#include")&&FirstChar(str)=='#') {
				if (HeadersCheck(str,line,filename))
					r=true;
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
				r=true;
			}
			else if (cmp(str,i,(char *)"freopen("))
				JudgeSettings::use_freopen=1;
			else
				for (int k=0;k<JudgeSettings::InvalidWords.size();k++)
					if (cmp(str,i,JudgeSettings::InvalidWords[k])) {
						if (JudgeSettings::Terminal) {
							ClearColor();
							HighLight();
						}
						printf("%s:%d: ",filename.c_str(),line);
						JudgeOutput::PrintError();
						printf("%s:%s\n",Context::InvalidWordFound,JudgeSettings::InvalidWords[k].c_str());
						r=true;
					}
	}
	fclose(fp);
	return r;
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
		tmp.MaxScore=100/filelist.size();
		if (!exist(tmp.stdOutput))
			continue;
		//cout << tmp.stdInput+" "+tmp.stdOutput << endl;
		point.push_back(tmp);
	}
	memorylimit=JudgeSettings::Default_memorylimit;
	timelimit=JudgeSettings::Default_timelimit;
}

JudgeResult Problem::JudgeProblem(Contestant oier){
	JudgeResult tot,tmpresult;
	tot.score=0;
	tot.st=AC;
	tot.memo=-1;
	tot.time=-1;
	putchar('\n');
	if (SafetyCheck(oier.name+name_to_print+".cpp")||(WEXITSTATUS(system(("g++ "+oier.name+name_to_print+".cpp -DEJUDGE -o ./"+name_to_print+((JudgeSettings::Terminal==0)?(string)" 2>.ejudge.tmp":(string)"")).c_str())))) {
		if (!JudgeSettings::Terminal) {
			fstream fin;
			fin.open(".ejudge.tmp");
			string line;
			while (getline(fin,line,'\n'))
				cout << line << endl;
			fin.close();
		}
		//JudgeOutput::PrintStatus(CE);
		vector<int> stat;
		for (int i=0;i<point.size();i++)
			stat.push_back(CE);
		return (JudgeResult){CE,0,0,0,stat};
	}
	int maxlength=-(1<<30);
	for (int i=0;i<point.size();i++)
		if ((int)point[i].stdInput.size()>maxlength)
			maxlength=point[i].stdInput.size();
	char tmp[256];
	sprintf(tmp,"%%%ds ",maxlength+3);
	for (int i=0;i<point.size();i++) {
		printf(tmp,point[i].stdInput.c_str());
		JudgeOutput::PrintResult(tmpresult=(point[i].JudgePoint("./"+name_to_print,timelimit,memorylimit)));
		tot.score+=tmpresult.score;
		if ((tot.st==AC)&&(tmpresult.st!=AC))
			tot.st=tmpresult.st;
		tot.stat.push_back(tmpresult.st);
		tot.memo=max(tot.memo,tmpresult.memo);
		tot.time=max(tot.time,tmpresult.time);
	}
	return tot;
}

void Contest::InitSPJ() {
}

void Contest::InitContest() {
	if (!PrinttoTerminal(JudgeSettings::PrintDevice))
		JudgeSettings::Terminal=0;
	vector<string> filelist=GetFile((string)".",(string)"\\.cpp");
	if (filelist.size()) {
		Contestant user;
		user.name="./";
		user.name_to_print=GetUserName();
		oier.push_back(user);
	}
	filelist=GetFile((string)"./source",(string)"");
	for (int i=0;i<filelist.size();i++) {
		Contestant tmp;
		tmp.name="./source/"+filelist[i]+"/";
		tmp.name_to_print=filelist[i];
		oier.push_back(tmp);
	}

	filelist=GetFile((string)".",(string)"\\.in");
	if (filelist.size()) {
		Problem Default;
		Default.name="./";
		Default.name_to_print=GetName();
		Default.InitProblem();
		problem.push_back(Default);
	}
	filelist=GetFile((string)"./data",(string)"");
	for (int i=0;i<filelist.size();i++) {
		Problem tmp;
		tmp.name="./data/"+filelist[i]+"/";
		tmp.name_to_print=filelist[i];
		tmp.InitProblem();
		problem.push_back(tmp);
	}
	
	//for (int i=0;i<oier.size();i++)
		//cout << oier[i].name_to_print+" "+oier[i].name << endl;
	//for (int i=0;i<problem.size();i++)
		//cout << problem[i].name_to_print+" "+problem[i].name << endl;
}

void Contest::JudgeContest() {
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
				oier[i].problem.push_back((JudgeResult){0,0,0,0});
			oier[i].problem[j]=problem[j].JudgeProblem(oier[i]);
			printf("\n%s ",oier[i].name_to_print.c_str());
			JudgeOutput::PrintResult(oier[i].problem[j]);
			printf("%s:%d\n\n",Context::Score,oier[i].problem[j].score);
		}
		oier[i].sumup();
	}
}

void JudgeOutput::PrintStatus(int st) {
	//color(Status_Color[st],black);
	if (JudgeSettings::Terminal) {	
		color(JudgeSettings::Status_Color[st],JudgeSettings::Status_Backround);
		HighLight();
		puts(JudgeSettings::Status[st]);
		ClearColor();
	}
	else
		puts(JudgeSettings::Status[st]);
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

void JudgeOutput::PrintResult(JudgeResult x) {
	if (JudgeSettings::Terminal) {
		foreground(green);
		printf("%s:",Context::Time);
		foreground(yellow);
		printf("%5.2lfs",x.time);
		foreground(green);
		printf(" %s:",Context::Memory);
		foreground(yellow);
		printf("%7dKB ",x.memo);
		PrintStatus(x.st);
	}
	else {
		printf("%s:%5.2lfs %s:%7dKB ",Context::Time,x.time,Context::Memory,x.memo);
		PrintStatus(x.st);
	}
}

void JudgeOutput::PrintName(string str,int len) {//修复输出问题
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

void JudgeOutput::OutputContest(Contest test) {
	sort(test.oier.begin(),test.oier.end());
	int len=6;
	for (int i=0;i<test.oier.size();i++)
		len=max(len,(int)test.oier[i].name_to_print.length());
	for (int i=0;i<len+1;i++)
		putchar(' ');
	for (int i=0;i<test.problem.size();i++)
		printf("%11s",test.problem[i].name_to_print.c_str());
	puts("");
	for (int i=0;i<test.oier.size();i++) {
		JudgeOutput::PrintName(test.oier[i].name_to_print,len);
		putchar(':');
		for (int j=0;j<test.problem.size();j++)
			printf("	%7d",test.oier[i].problem[j].score);
		puts("");
	}
#ifndef DEBUG
	if (JudgeSettings::PrinttoCSV)
#endif
		ConverttoCSV(test,(string)"result.csv");
}

void JudgeOutput::ConverttoCSV(Contest test,string csv) {
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
			for (int k=0;k<test.oier[i].problem[j].stat.size();k++)
				fout << JudgeSettings::Status_Short[test.oier[i].problem[j].stat[k]];
			fout << ',' << test.oier[i].problem[j].score << ',';
		}
		fout << endl;
	}
	fout << ',' << endl;
	for (int i=0;i<3+test.problem.size()*2;i++)
		fout << ',';
	fout << "\"" << Context::SymbolExplanation << "\"" << endl;
	for (int i=0;i<6;i++)
		fout << JudgeSettings::Status_Short[i] << ":" << JudgeSettings::Status[i] << endl;
	fout << "\"," << endl;
}
