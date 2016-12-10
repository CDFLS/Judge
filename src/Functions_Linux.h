#ifndef FUNCTIONSLINUX_H
#define FUNCTIONSLINUX_H 
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
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
#endif
