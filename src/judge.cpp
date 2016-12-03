#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include "judge_test.h"
#include "Conio.h"
#include "Functions_Linux.h"
#include "judge.h"
using namespace std;

const char Status[][40]={"Accepted","Wrong Answer","Compile Error","Time Limit Exceeded","Memory Limit Exceeded","Runtime Error"};
const char Status_Color[]={green,red,yellow,red,red,yellow};//输出Accepted等提示信息的颜色
char Dict[100][20]={"windows.h","system(","fopen(","<con>","rand()","!"};//禁用单词。"!"标志数组的结束
int Arg_c=0;//-c选项标志
int Flag_Freopen=0;//文件输入输出
double timelimit=1.0;
int memorylimit=128000;
char name[512];

void print(int st) {//输出Accepted等提示。高亮+彩显
    color(Status_Color[st],black);
    HighLight();
    puts(Status[st]);
    ClearColor();
}

void PrintResult(result x) {//输出Judge结果
    foreground(green);
    printf("Time:");
    foreground(yellow);
    printf("%5.2lfs",x.time);
    foreground(green);
    printf(" Memory:");
    foreground(yellow);
    printf("%7dKB ",x.memo);
    print(x.s);
}

bool cmp(char *str,int s,char *word) {//比较字符串，从str的第s个和word的第1个字符开始比较
    if (strlen(str)-s<strlen(word))
        return false;
    for (int i=0;i<strlen(word);i++)
        if (str[s+i]!=word[i])
            return false;
    return true;
}

int Args(int c,char *v[]) {//解析命令行参数
    for (int i=1;i<c;i++)
        if (v[i][0]=='-') {
            i++;
            if (cmp(v[i-1],0,(char *)"-t")&&(strlen(v[i-1])==2)&&(i<c))//时间
                sscanf(v[i],"%lf",&timelimit);
            else if (cmp(v[i-1],0,(char *)"-m")&&(strlen(v[i-1])==2)&&(i<c))//内存
                sscanf(v[i],"%d",&memorylimit);
            else if (cmp(v[i-1],0,(char *)"-w")&&(strlen(v[i-1])==2)&&(i<c)) {//添加禁用单词，如-w3表示添加接下的的三个单词，-w与-w1等效
                int last,t;
                for (last=0;Dict[last][0]!='!';last++) ;
                if (sscanf(v[i-1],"-w%d",&t)==-1)
                    t=1;
                for (int k=0;(k<t)&&(i<c);k++)
                    sprintf(Dict[last+k],"%s",v[i]),i++;
                Dict[last+t][0]='!';
            }
            else if (cmp(v[i-1],0,(char *)"-c")&&(strlen(v[i-1])==2))
                Arg_c=1;
            else if ((cmp(v[i-1],0,(char *)"-a")&&(strlen(v[i-1])==2))||(cmp(v[i-1],0,(char *)"--all")&&(strlen(v[i-1])==5))) {
                if (i!=2)
                    continue;
                judge_test(c,v);
                return 1;
            }
            else if ((cmp(v[i-1],0,(char *)"-h")&&(strlen(v[i-1])==2))||(cmp(v[i-1],0,(char *)"--help")&&(strlen(v[i-1])==6))) {
                cout<< "用法：judge [选项]... [文件前缀]" << endl
                    << "评测OI程序，编译指定文件前缀(若未指定则使用当前目录名)，并使用前缀相同的输入输出文件(自动查找)评测。文件操作自动检测，但仅允许freopen。" << endl
                    << "编译命令:g++ [FILENAME].cpp -o [FILENAME] -DEJUDGE" << endl
                    << endl
                    << "    -w [STRING]               禁止源文件中出现该字符串" << endl
                    << "    -w[NUMBER] [STRING]...    禁止源文件中出现以下NUMBER个字符串" << endl
                    << "    -t [TIME]                 限定程序运行时间(未指定时为" << timelimit << "s)" << endl
                    << "    -m [MEMORY]               限制程序使用内存(为指定时为" << memorylimit << "KB)" << endl
                    << "    -h, --help                显示本帮助" << endl
                    << "    -c                        只编译，不测试" << endl
                    << "    -a, --all                 评测一次考试，必须作为第一个参数。" << endl
                    << endl
                    << "当程序超出限定时间时会被强制结束，但超出限定内存时并不会，因此有可能出现MLE的程序被判定为RE的情况。" << endl;
                return 1;
            }
            else{
                printf("judge: 未知的选项或不足的参数 '%s'\n请尝试 \"judge --help\"，以获得更多信息。\n",v[i-1]);
                return 1;
            }
        }
        else{
            sprintf(name,"%s",v[i]);
            for (int k=strlen(name)-1;k>=0;k--)
                if ((name[k]=='\n')||(name[k]=='\r'))
                    name[k]=0;
                else
                    break;
        }
    return 0;
}

bool exist(char * filename) {//检测文件是否存在
    FILE *fp=fopen(filename,"r");
    if (fp==NULL)
        return false;
    fclose(fp);
    return true;
}

bool SafetyCheck() {//检测是否有禁用单词
    int r=false;
    char str[2048];
    sprintf(str,"%s.cpp",name);
    FILE *fp=fopen(str,"r");
    if (fp==NULL) {
        foreground(red);
        HighLight();
        printf("Error:");
        ClearColor();
        puts("source not found.");
        return true;
    }
    int line=0,ifndef=0,flag=0,ifdef=0;//忽略在注释和#ifndef EJUDGE和#ifdef EJUDGE #else中的单词
    while (fgets(str,2000,fp)!=NULL) {
        if (str[strlen(str)-1]=='\n')
            str[strlen(str)-1]=0;
        line++;
        int include=0;
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
            else if (cmp(str,i,(char *)"#include"))
                include=1;
            else if (str[i]=='"'&&include) {//不允许调用自定义头文件
                foreground(red);
                HighLight();
                printf("Error:");
                ClearColor();
                printf("invalid head file at line %d:%s\n",line,str);
                r=true;
            }
            else if (cmp(str,i,(char *)"freopen("))
                Flag_Freopen=1;
            else
                for (int k=0;Dict[k][0]!='!';k++)
                    if (cmp(str,i,Dict[k])) {
                        foreground(red);
                        HighLight();
                        printf("Error:");
                        ClearColor();
                        printf("invalid word found at line %d:%s\n",line,Dict[k]);
                        r=true;
                    }
    }
    fclose(fp);
    return r;
}

bool Compile() {//编译程序
    puts("Compiling...");
    if (SafetyCheck()) {
        ClearFile();
        print(CE);
        return true;
    }
    char str[512];
    sprintf(str,"g++ %s.cpp -o %s -O2 -DEJUDGE",name,name);
    if (WEXITSTATUS(system(str))) {
        print(CE);
        ClearFile();
        return true;
    }
    puts("done.");
    return Arg_c==1;
}

result judge(char *in,char *out) {//评测单个测试点
    int s=AC,memo;
    double time;
    char str[512];
    if (Flag_Freopen) {
        sprintf(str,"cp %s %s.in",in,name);
        system(str);
        sprintf(str,"time -f \"Time:%%es Memory:%%MKB\" timeout --signal=KILL %lfs ./%s 2>.ejudge.run",timelimit,name);
        system(str);
        sprintf(str,"mv %s.out .ejudge.tmp 2>/dev/null",name);
        system(str);
        sprintf(str,"rm %s.in",name);
        system(str);
    } else {
        sprintf(str,"time -f \"Time:%%es Memory:%%MKB\" timeout --signal=KILL %lfs ./%s < %s > .ejudge.tmp 2>.ejudge.run",timelimit,name,in);//为time命令指定格式获取用时和内存使用，并用timeout命令限制运行时间。
        system(str);
    }
    FILE *fp=fopen(".ejudge.run","r");
    char ch;
//解析输出。
//当程序超时，timeout终结进程时，输出如下：
//      Command terminated by signal 9
//      Time:1.00s Memory:1732KB
//当程序运行错误时，输出如下：
//      timeout: the monitored command dumped core
//      Command terminated by signal 11
//      Time:0.02s Memory:1924KB
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
        return (result){TLE,memo,time};
    if (s==RE)
        return (result){RE,0,0};
    if (memo>memorylimit)
        return (result){MLE,memo,time};
    if (memo==0)
        return (result){RE,memo,time};
    sprintf(str,"timeout 1s diff -b -B -Z .ejudge.tmp %s > /dev/null 2>/dev/null",out);//比较输出，忽略由空格数不同造成的差异，忽略任何因空行而造成的差异，忽略每行末端的空格。更多用法用法参见diff --help，此设置应在大多数情况下有效。
    if (WEXITSTATUS(system(str))!=0)
        return (result){WA,memo,time};
    else
        return (result){AC,memo,time};
}

int judge_single()
{
    int length=-1,FileChanged=0;
    char in[512],out[512],temp[512];
    if (Flag_Freopen) {
        sprintf(temp,"%s.in",name);
        if (exist(temp)) {
            FileChanged=1;
            sprintf(temp,"mv %s.in %s_.in&&mv %s.out %s_.out 2>/dev/null",name,name,name,name);
            system(temp);
        }
    }

    int score=0,tot=0,st=0,memotot=0;
    double timetot=0;
    InitFile();
    FILE *infile=fopen(".ejudge.input","r");
    while (fgets(in,500,infile)!=NULL) {
        for (int i=strlen(in)-1;in[i]=='\n';i--)
            in[i]=0;
        int k;
        for (k=0;(k<strlen(in))&&(in[k]!='.');k++)
            temp[k]=in[k];
        temp[k]=0;
        sprintf(out,"%s.out",temp);
        if (!exist(out)) {
            sprintf(out,"%s.ans",temp);
            if (!exist(out))
                continue;
        }
        if (length==-1)
            length=strlen(in);
        tot++;
        foreground(yellow);
        sprintf(temp,"%%%ds: ",length+3);
        printf(temp,in);
        result tmp=judge(in,out);
        PrintResult(tmp);
        if (tmp.s==AC)
            score++;
        else if (st==AC)
            st=tmp.s;
        timetot+=tmp.time;
        memotot+=tmp.memo;
    }
    if (FileChanged) {
        sprintf(temp,"mv %s_.in %s.in&&mv %s_.out %s.out 2>/dev/null",name,name,name,name);
        system(temp);
    }
    fclose(infile);
    if (tot==0) {
        foreground(red);
        HighLight();
        printf("Error:");
        ClearColor();
        puts("no input file found.");
        ClearFile();
        return 0;
    }
    foreground(green);
    printf("Time: ");
    foreground(blue);
    printf("%4.2lfs",timetot);
    foreground(green);
    printf(" Memory:");
    foreground(blue);
    printf("%6dKB\n",memotot);
    foreground(green);
    printf("Score: ");
    foreground(yellow);
    printf("%d\n",score*100/tot);
    print(st);
    return score*100/tot;
}
