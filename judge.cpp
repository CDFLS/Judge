#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "Conio+.h"
using namespace std;

#define AC 0
#define WA 1
#define CE 2
#define TLE 3
#define MLE 4
#define RE 5

struct result{
    int s,memo;
    double time;
};//Judge的结果

//输出格式。
const char Status[][40]={"Accepted","Wrong Answer","Compile Error","Time Limit Exceeded","Memory Limit Exceeded","Runtime Error"};
const char Status_Color[]={green,red,yellow,red,red,yellow};
char Dict[100][20]={"windows.h","system(","freopen(","fopen(","<con>","!"};
//禁用单词。"!"标志数组的结束。

double timelimit=1.0;
int memorylimit=128000;
char name[512];

void HighLight()
{//高亮显示。
    printf("\033[1m");
}

inline void ClearColor()
{//清除颜色。
    printf("\033[0m");
}

inline void ClearFile()
{//清理文件。
    system("rm ./.ejudge.*");
}

void print(int st)
{//输出Accepted等提示。高亮+彩显
    color(Status_Color[st],black);
    HighLight();
    puts(Status[st]);
    ClearColor();
}

void PrintResult(result x)
{//输出Judge结果
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

void GetName()
{//获取当前目录名
    system("basename $PWD > .ejudge.tmp");
    FILE *fp=fopen(".ejudge.tmp","r");
    fscanf(fp,"%s",name);
    for (int i=strlen(name)-1;i>=0;i--)
        if ((name[i]=='\n')||(name[i]=='\r'))
            name[i]=0;
        else
            return ;
}

void Args(int c,char *v[])
{//解析命令行参数
    for (int i=1;i<c;i++)
        if (v[i][0]=='-')
        {
            i++;
            if (v[i-1][1]=='t')//时间
                sscanf(v[i],"%lf",&timelimit);
            if (v[i-1][1]=='m')//内存
                sscanf(v[i],"%d",&memorylimit);
            if (v[i-1][1]=='w')//添加禁用单词，如-w3表示添加接下的的三个单词，-w与-w1等效。
            {
                int last,t;
                for (last=0;Dict[last][0]!='!';last++) ;
                if (sscanf(v[i-1],"-w%d",&t)==-1)
                    t=1;
                for (int k=0;k<t;k++)
                    sprintf(Dict[last+k],"%s",v[i]),i++;
                Dict[last+t][0]='!';
            }
        }else{
            sprintf(name,"%s",v[i]);
            for (int k=strlen(name)-1;k>=0;k--)
                if ((name[k]=='\n')||(name[k]=='\r'))
                    name[k]=0;
                else
                    break;
        }
}

bool exist(char * filename)
{//检测文件是否存在
    FILE *fp=fopen(filename,"r");
    if (fp==NULL)
        return false;
    fclose(fp);
    return true;
}

bool cmp(char *str,int s,char *word)
{//比较字符串，从str的第s个和word的第1个字符开始比较
    if (strlen(str)-s<strlen(word))
        return false;
    for (int i=0;i<strlen(word);i++)
        if (str[s+i]!=word[i])
            return false;
    return true;
}

bool SafetyCheck()
{//检测是否有禁用单词
    int r=false;
    char str[2048];
    sprintf(str,"%s.cpp",name);
    FILE *fp=fopen(str,"r");
    if (fp==NULL)
    {
        foreground(red);
        HighLight();
        printf("Error:");
        ClearColor();
        puts("source file not found.");
        return true;
    }
    int line=0,ifdef=0,flag=0;//忽略在注释和#ifndef EJUDGE中的单词
    while (fgets(str,2000,fp)!=NULL)
    {
        if (str[strlen(str)-1]=='\n')
            str[strlen(str)-1]=0;
        line++;
        int include=0;
        for (int i=0;i<strlen(str);i++)
            if ((str[i]=='/')&&(str[i+1]=='/'))
                break;
            else if ((str[i]=='*')&&(str[i+1]=='/'))
                flag=0;
            else if (cmp(str,i,(char *)"#ifndef EJUDGE"))
                ifdef=1;
            else if (cmp(str,i,(char *)"#endif")||cmp(str,i,(char *)"#else"))
                ifdef=0;
            else if (flag||ifdef)
                continue;
            else if ((str[i]=='/')&&(str[i+1]=='*'))
                flag=1;
            else if (cmp(str,i,(char *)"#include"))
                include=1;
            else if (str[i]=='"'&&include)//不允许调用自定义头文件
            {
                foreground(red);
                HighLight();
                printf("Error:");
                ClearColor();
                printf("invalid head file at line %d:%s\n",line,str);
                r=true;
            }
            else
                for (int k=0;Dict[k][0]!='!';k++)
                    if (cmp(str,i,Dict[k]))
                    {
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

bool Compile()
{//编译程序
    puts("Compiling...");
    if (SafetyCheck())
    {
        ClearFile();
        print(CE);
        return true;
    }
    char str[512];
    sprintf(str,"g++ %s.cpp -o %s -O2 -DEJUDGE",name,name);
    if (WEXITSTATUS(system(str)))
    {
        print(CE);
        ClearFile();
        return true;
    }
    puts("done.");
    return false;
}

result judge(char *in,char *out)
{//评测单个测试点
    int s=AC,memo;
    double time;
    char str[512];
    sprintf(str,"/bin/time -f \"Time:%%es Memory:%%MKB\" timeout %lfs ./%s < %s > .ejudge.tmp 2>.ejudge.run",timelimit,name,in);//为time命令指定格式获取用时和内存使用，并用timeout命令限制运行时间。
    system(str);
    FILE *fp=fopen(".ejudge.run","r");
    char ch;
//解析输出。
//当程序超时，timeout终结进程时，输出如下：
//      Command exited with non-zero status 124
//      Time:1.00s Memory:1732KB
//当程序运行错误时，输出如下：
//      timeout: the monitored command dumped core
//      Command terminated by signal 11
//      Time:0.02s Memory:1924KB
//故有如下解析代码：
    fscanf(fp,"%c",&ch);
    if (ch=='C')
    {
        s=TLE;
        while (ch!='T')
            fscanf(fp,"%c",&ch);
    }
    if (ch=='t')
    {
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
    sprintf(str,"diff -b -B -Z .ejudge.tmp %s > /dev/null",out);//比较输出，忽略由空格数不同造成的差异，忽略任何因空行而造成的差异，忽略每行末端的空格。更多用法用法参见diff --help，此设置应在大多数情况下有效。
    if (WEXITSTATUS(system(str))==1)
        return (result){WA,memo,time};
    else
        return (result){AC,memo,time};
}

int main(int argc,char *argv[])
{
    GetName();
    Args(argc,argv);
    if (Compile())
        return 0;
    int score=0,tot=0,st=0,maxmemo=0;
    double maxtime=0;
    for (int i=0;i<=20;i++)
    {
        char in[512],out[512];
        sprintf(in,"%s%d.in",name,i);
        sprintf(out,"%s%d.out",name,i);
        if (!exist(in))
            continue;
        if (!exist(out))
        {
            sprintf(out,"%s%d.ans",name,i);//如果不存在.out文件，尝试.ans后缀。
            if (!exist(out))
                continue;
        }
        tot++;
        foreground(yellow);
        if (i<10)
            putchar(' ');
        printf("    %s: ",in);
        result tmp=judge(in,out);
        PrintResult(tmp);
        if (tmp.s==AC)
            score++;
        else if (st==AC)
            st=tmp.s;
        maxtime=max(maxtime,tmp.time);
        maxmemo=max(maxmemo,tmp.memo);
    }
    if (tot==0)
    {
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
    printf("%4.2lfs",maxtime);
    foreground(green);
    printf(" Memory:");
    foreground(blue);
    printf("%6dKB\n",maxmemo);
    foreground(green);
    printf("Score: ");
    foreground(yellow);
    printf("%d\n",score*100/tot);
    print(st);
    ClearColor();
    ClearFile();
    return 0;
}
