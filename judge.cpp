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
};

const char Status[][40]={"Accepted","Wrong Answer","Compile Error","Time Limit Exceeded","Memory Limit Exceeded","Runtime Error"};
const char Status_Color[]={green,red,yellow,red,red,yellow};
char Dict[100][20]={"windows.h","system(","freopen(","fopen(","<con>","!"};//the symbol '!' indicates the end of this array

double timelimit=1.0;
int memorylimit=128000;
char name[512];

inline void ClearColor()
{
    printf("\033[0m");
}

inline void ClearFile()
{
    system("rm ./.ejudge.*");
}

void print(int st)
{
    color(Status_Color[st],white);
    puts(Status[st]);
    ClearColor();
}

void PrintResult(result x)
{
    foreground(green);
    printf("Time:");
    foreground(yellow);
    printf("%5.2lfs",x.time);
    foreground(green);
    printf(" Memory:");
    foreground(yellow);
    printf("%6dKB ",x.memo);
    print(x.s);
}

void GetName()
{
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
{
    for (int i=1;i<c;i++)
        if (v[i][0]=='-')
        {
            i++;
            if (v[i-1][1]=='t')
                sscanf(v[i],"%lf",&timelimit);
            if (v[i-1][1]=='m')
                sscanf(v[i],"%d",&memorylimit);
            if (v[i-1][1]=='w')
            {
                int last,t;
                for (last=0;Dict[last][0]!='!';last++) ;
                sscanf(v[i-1],"-w%d",&t);
                for (int k=0;k<t;k++)
                    sprintf(Dict[last+k],"%s",v[i]),i++;
                Dict[last+t][0]='!';
            }
        }
        else
            sprintf(name,"%s",v[i]);
}

bool exist(char * filename)
{
    FILE *fp=fopen(filename,"r");
    if (fp==NULL)
        return false;
    fclose(fp);
    return true;
}

bool cmp(char *str,int s,char *word)
{
    if (strlen(str)-s<strlen(word))
        return false;
    for (int i=0;i<strlen(word);i++)
        if (str[s+i]!=word[i])
            return false;
    return true;
}

bool SafetyCheck()
{
    int r=false;
    char str[2048];
    sprintf(str,"%s.cpp",name);
    FILE *fp=fopen(str,"r");
    int line=0,ifdef=0;
    while (fgets(str,2000,fp)!=NULL)
    {
        if (str[strlen(str)-1]=='\n')
            str[strlen(str)-1]=0;
        line++;
        int flag=0,include=0;
        for (int i=0;i<strlen(str);i++)
            if ((str[i]=='/')&&(str[i+1]=='/'))
                break;
            else if ((str[i]=='*')&&(str[i+1]=='/'))
                flag=0;
            else if ((str[i]=='#')&&(str[i+1]=='i')&&(str[i+2]=='f'))
                ifdef=1;
            else if ((str[i]=='#')&&(str[i+1]=='e')&&(str[i+2]=='n'))
                ifdef=0;
            else if (flag||ifdef)
                continue;
            else if ((str[i]=='/')&&(str[i+1]=='*'))
                flag=1;
            else if ((str[i]=='#')&&(str[i+1]=='i')&&(str[i+2]=='n')&&(str[i+3]=='c'))
                include=1;
            else if (str[i]=='"'&&include)
            {
                foreground(red);
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
                        printf("Error:");
                        ClearColor();
                        printf("invalid words found at line %d:%s\n",line,Dict[k]);
                        r=true;
                    }
    }
    fclose(fp);
    return r;
}

bool Compile()
{
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
{
    int s=AC,memo;
    double time;
    char str[512];
    sprintf(str,"/bin/time -f \"Time:%%es Memory:%%MKB\" timeout %lfs ./%s < %s > .ejudge.tmp 2>.ejudge.run",timelimit,name,in);
    system(str);
    FILE *fp=fopen(".ejudge.run","r");
    char ch;
    fscanf(fp,"%c",&ch);
    if (ch=='C')
    {
        s=TLE;
        while (ch!='T')
            fscanf(fp,"%c",&ch);
    }
    fscanf(fp,"ime:%lfs Memory:%dKB",&time,&memo);
    fclose(fp);
    if (s==TLE)
        return (result){TLE,memo,time};
    if (memo>128000)
        return (result){MLE,memo,time};
    if (memo==0)
        return (result){RE,memo,time};
    sprintf(str,"diff -w -B -Z --strip-trailing-cr .ejudge.tmp %s > /dev/null",out);
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
            sprintf(out,"%s%d.ans",name,i);
            if (!exist(out))
                continue;
        }
        tot++;
        foreground(yellow);
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
