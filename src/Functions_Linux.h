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
void HighLight();
void ClearColor();
void ClearFile();
vector<string> GetFile(const char * path,const char * grep);
vector<string> GetDir(const char *path);
bool existdir(const char *path);
string GetName();
string GetUserName();
string GetJudgerc();
bool STRCMP(char *str);
bool PrinttoTerminal(string &x);
string BaseName(string &x);
#endif
