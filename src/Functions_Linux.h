#ifndef FUNCTIONSLINUX_H
#define FUNCTIONSLINUX_H 
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
static void HighLight() {//高亮显示
    printf("\033[1m");
}

static inline void ClearColor() {//清除颜色
    printf("\033[0m");
}

static inline void ClearFile() {//清理文件
    system("rm ./.ejudge.* 2>/dev/null");
}

static vector<string> GetFile(string directory,string grep) {
    system(("ls "+directory+" 2>/dev/null|grep \""+grep+"$\" > .ejudge.input 2>/dev/null").c_str());
	vector<string> ans;
	string tmp;
	ifstream fin;
	fin.open(".ejudge.input");
	while (fin) {
		getline(fin,tmp,'\n');
		ans.push_back(tmp);
	}
	ans.erase(ans.end()-1);
	fin.close();
	return ans;
}

static vector<string> GetDir(const char *path) {
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

static bool existdir(const char *path) {
	DIR *pDir;
	pDir=opendir(path);
	if (pDir==NULL)
		return false;
	closedir(pDir);
	return true;
}

static string GetName() {//获取当前目录名
    system("basename \"$PWD\" > .ejudge.tmp 2>/dev/null");
	ifstream fin;
	string tmp;
	fin.open(".ejudge.tmp");
	getline(fin,tmp,'\n');
	fin.close();
	return tmp;
}

static string GetUserName() {
    system("echo \"$USERNAME\" > .ejudge.tmp");
	ifstream fin;
	string tmp;
	fin.open(".ejudge.tmp");
	getline(fin,tmp,'\n');
	fin.close();
	return tmp;
}
static string GetJudgerc() {
	system("echo \"$HOME/.judgerc\">.ejudge.tmpname");
	string filename;
	fstream fin;
	fin.open(".ejudge.tmpname");
	getline(fin,filename,'\n');
	fin.close();
	return filename;
}

static bool STRCMP(char *str) {
	const char s[]="/dev/";
	for (int i=0;i<min(strlen(s),strlen(str));i++)
		if (str[i]!=s[i])
			return true;
	return false;
}

static bool PrinttoTerminal(string &x) {
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

static string BaseName(string &x) {
	string tmp;
	for (int i=x.length()-1;i>=0;i--)
		if (x[i]=='/') {
			for (int j=i+1;j<=x.length()-1;j++)
				tmp.push_back(x[j]);
			return tmp;
		}
	return tmp;
}
#endif