#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

using namespace std;
void HighLight() {//高亮显示
    printf("\033[1m");
}

void ClearColor() {//清除颜色
    printf("\033[0m");
}

void ClearFile() {//清理文件
    system("rm ./.ejudge.* 2>/dev/null");
}

vector<string> GetFile(const char * path,const char * grep) {
    vector<string> ans;
    DIR *pDir;
    struct dirent *ent;
    pDir=opendir(path);
    if (pDir==NULL)
        return (vector<string>){};
    while ((ent=readdir(pDir))!=NULL)
        if (!(ent->d_type & DT_DIR)) {
            string tmp=ent->d_name;
            if ((tmp.length()>=strlen(grep))&&(tmp.substr(tmp.length()-strlen(grep),strlen(grep))==(string)grep))
                ans.push_back(tmp);
        }
    closedir(pDir);
    return ans;
}

vector<string> GetDir(const char *path) {
    vector<string> ans;
    DIR *pDir;
    struct dirent *ent;
    pDir=opendir(path);
    if (pDir==NULL)
        return (vector<string>){};
    while ((ent=readdir(pDir))!=NULL)
        if ((ent->d_type & DT_DIR)&&((string)ent->d_name!=".")&&((string)ent->d_name)!="..")
            ans.push_back((string)ent->d_name);
    closedir(pDir);
    return ans;
}

bool existdir(const char *path) {
    DIR *pDir;
    pDir=opendir(path);
    if (pDir==NULL)
        return false;
    closedir(pDir);
    return true;
}

string GetName() {//获取当前目录名
    FILE * s=popen("basename \"$PWD\"","r");
    char str[1024];
    fgets(str,sizeof(str),s);
    str[strlen(str)-1]=0;
    pclose(s);
    return (string)str;
}

string GetUserName() {
    FILE * s=popen("echo \"$USERNAME\"","r");
    char str[1024];
    fgets(str,sizeof(str),s);
    str[strlen(str)-1]=0;
    pclose(s);
    return (string)str;
}
string GetJudgerc() {
    FILE * s=popen("echo \"$HOME/.judgerc\"","r");
    char str[1024];
    fgets(str,sizeof(str),s);
    str[strlen(str)-1]=0;
    pclose(s);
    return (string)str;
}

bool STRCMP(char *str) {
    const char s[]="/dev/";
    for (int i=0;i<min(strlen(s),strlen(str));i++)
        if (str[i]!=s[i])
            return true;
    return false;
}

bool PrinttoTerminal(string &x) {
        int fd = fileno(stdout);
        char s[256] = {0}, name[256] = {0};
        snprintf(s, 255, "/proc/%d/fd/%d", getpid(), fd);
        readlink(s, name, 255);
        x=name;
        if (STRCMP(name))
            return false;
        else
            return true;
}

string BaseName(string &x) {
    string tmp;
    for (int i=x.length()-1;i>=0;i--)
        if (x[i]=='/') {
            for (int j=i+1;j<=x.length()-1;j++)
                tmp.push_back(x[j]);
            return tmp;
        }
    return x;
}
