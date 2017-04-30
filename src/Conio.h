#ifndef CONIOPLUS_H
#define CONIOPLUS_H

#if defined(linux) || defined(__APPLE__)
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

#define black 0
#define blue 4
#define green 6
#define cyan 2
#define red 1
#define purple 5
#define yellow 3
#define white 7

bool kbhit();
int getch(void);
#else

#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#define HighLight()
#define ClearColor()
using namespace std;
#define black 8
#define blue 9
#define green 10
#define cyan 11
#define red 12
#define purple 13
#define yellow 14
#define white 15

#endif

void gotoxy(int x,int y);
void hidecursor();
void unhidecursor();
void color(int a,int b);
void foreground(int a);
void background(int a);
double pro_time();
int readkey(double _time__);
void clearcolor();
void clear();
bool exist(string filename);
#endif
