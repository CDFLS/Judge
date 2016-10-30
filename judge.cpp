#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "Conio+.h"
char str[256];
int ans=0,tot=0,tle=0,flag,mle=0,flagmle;
bool exist(int x)
{
    char s[512];
    sprintf(s,"%s%d.in",str,x);
    FILE *fp=fopen(s,"r");
    if (fp==NULL)
        return false;
    fclose(fp);
    return true;
}
void CheckRun(int number)
{
    foreground(blue);
    printf("    Test%d:",number);
    foreground(yellow);
    FILE *fp=fopen(".run","r");
    char ch;
    fscanf(fp,"%c",&ch);
    if (ch=='C')
    {
        tle++;
        flag=1;
        printf("Terminated.");
        return ;
    }
    double t;int m;
    fscanf(fp,"ime:%lfs Memory:%dKB",&t,&m);
    if (t>1.0)
    {
        tle++;
        flag=1;
    }
    if (m>128000)
    {
        mle++;
        flag=flagmle=1;
    }
    fclose(fp);
    printf(" Time:%5.2lfs Memory:%7dKB",t,m);
}
int main()
{
    system("basename $PWD>your_output");
    FILE *fp=fopen("your_output","r");
    fscanf(fp,"%s",str);
    fclose(fp);
    for (int i=strlen(str)-1;i>=0;i--)
        if ((str[i]=='\n')||(str[i]=='\r'))
            str[i]=0;
        else
            break;
    char s[512],s1[512];
    printf("Compiling...\n");
    sprintf(s,"g++ %s.cpp -o %s -Wall",str,str);
    if (WEXITSTATUS(system(s)))
    {
        color(red,white);
        puts("Compile Error");
        return 0;
    }
    printf("done.\n");
    for (int i=0;i<21;i++)
    {
        if (!exist(i))
            continue;
        sprintf(s,"%s%d.in",str,i);
        sprintf(s1,"/bin/time -f \"Time:%%es Memory:%%MKB\" ./%s < %s > your_output 2>.run",str,s);
        system(s1);
        flag=flagmle=0;
        CheckRun(i);
        sprintf(s,"diff -w -B your_output %s%d.out > /dev/null",str,i);
        foreground(red);
        if (!flag)
        {
            if (WEXITSTATUS(system(s))==0)
                foreground(green),puts(" Accepted"),ans++;
            else
                puts(" Wrong Answer");
        }
        else
        {
            if (flagmle)
                puts(" Memory Limit Exceeded");
            else
                puts(" Time Limit Exceeded");
        }
        tot++;
    }
    foreground(red);
    system("rm your_output");
    system("rm .run 2>/dev/null");
    if (tot==0)
    {
        puts("Input file not found.");
        return 0;
    }
    else
        printf("Score: %d\n",ans*100/tot);
    color(red,white);
    if (tle>0)
        puts("Time Limit Exceeded");
    else if (mle>0)
        puts("Memory Limit Exceeded");
    else if (ans==tot)
        puts("Accepted");
    else
        puts("Wrong Answer");
    return 0;
}
