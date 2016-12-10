#ifndef CONIO_H
#define CONIO_H 
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#define Hblack 0
#define Hblue 4
#define Hgreen 9
#define Hcyan 2
#define Hred 1
#define Hpurple 5
#define Hyellow 3
#define Hwhite 7

#define black 0
#define blue 4
#define green 6
#define cyan 2
#define red 1
#define purple 5
#define yellow 3
#define white 7

static int kbhit(void) {
	struct termios oldt, newt;
	int ch;
	int oldf;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if(ch != EOF) {
		ungetc(ch, stdin);
		return 1;
	}
	return 0;
}

static int getch(void) {
     struct termios tm, tm_old;
     int fd = 0, ch;
     if (tcgetattr(fd, &tm) < 0) {//保存现在的终端设置
          return -1;
     }
     tm_old = tm;
     cfmakeraw(&tm);//更改终端设置为原始模式，该模式下所有的输入数据以字节为单位被处理
     if (tcsetattr(fd, TCSANOW, &tm) < 0) {//设置上更改之后的设置
          return -1;
     }
     ch = getchar();
     if (tcsetattr(fd, TCSANOW, &tm_old) < 0) {//更改设置为最初的样子
          return -1;
     }
     return ch;
}

static inline void gotoxy(int x,int y) {
	printf("\033[%d;%dH",y,x);
}

static inline void HideCursor() {
	printf("\033[?25l");
}

static inline void UnHideCursor() {
	printf("\033[?25h");
}

static inline void color(int a,int b) {
	printf("\033[%dm\033[%dm",b+40,a+30);
}

static inline void foreground(int a) {
    printf("\033[%dm",a+30);
}

static inline void background(int a) {
    printf("\033[%dm",a+40);
}

static inline int random(int m) {
	return (int)((double)rand()/RAND_MAX*(m-1)+0.5);
}

static double pro_time() {
	return (double)clock()/CLOCKS_PER_SEC;
}

static int readkey(double _time__) {
	double time_last=pro_time();
	while (!kbhit()&&(pro_time()-time_last<_time__))  ;
	if (!kbhit()) return 0;
	while (pro_time()-time_last<_time__)  ;
	int return_=getch();
	return (return_==224)?getch():return_;
}

static bool exist(string str) {
	fstream fin;
	fin.open(str);
	if (!fin)
		return false;
	fin.close();
	return true;
}
#endif
