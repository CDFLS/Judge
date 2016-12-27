#ifndef LANG
#define LANG
#include "../judge.h"
#include "../ConverttoString.h"
#include "../MultiCompiler.h"
#include <sstream>
#include <iostream>
using namespace std;
namespace Context {
	static string CUIhelp="tab键切换模式(重测和查看记录)，ws上下移动，ad翻页，enter选择,q退出";
	static void PrintHelp() {
		string help=
"用法：judge [选项]...\n\
评测OI程序，编译当前目录下和source/[选手名]目录下的代码(使用该代码父目录名)，并使用当前目录下和data/[题目名]目录下的数据(自动查找)评测。文件操作自动检测(仅检测freopen)。\n\
当以root权限运行judge时，将调用chroot实现一个简单的沙盒。\n\
代码检测仅检测C与C++\n\
编译使用命令:\n";
		for (int i=0;i<CompileCommand.size();i++)
			help+=CompileCommand[i]+"\n";
		help+="\n\
    -f [STRING]               禁止源文件中出现该函数\n\
    -f[NUMBER] [STRING]...    禁止源文件中出现以下NUMBER个函数\n\
    -h [STRING]               禁止源文件中出现该头文件\n\
    -h[NUMBER] [STRING]...    禁止源文件中出现以下NUMBER个头文件\n\
    -t [TIME]                 限定程序运行时间(未指定时为" + CTS::DoubleToString(JudgeSettings::Default_timelimit) + "s)\n\
    -m [MEMORY]               限制程序使用内存(为指定时为" + CTS::IntToString(JudgeSettings::Default_memorylimit) + "KB)\n\
    --csv                     输出结果到result.csv，当人数大于一时默认开启\n\
    --nocsv                   禁止输出到result.csv(即使人数大于一)\n\
    -c, --cui                 使用一个简单的CUI管理一次考试\n\
    --help                    显示本帮助\n\
\n\
全局配置文件：\n\
程序会依次从~/.judgerc和./judgerc中读取设置，设置文件格式为：[选项]([参数1],[参数2]......)\n\
当参数为字符串且参数不定时，会启用转义符，如\"\\,\"表示\",\"，\"\\(\"表示\"(\"目前支持以下选项：\n\
  无参数：\n\
    quit                      回到全局配置编辑模式(在enter命令结束时使用)\n\
  一个参数：\n\
    background, bg            设置输出AC、WA等的背景色，有以下值可选：black green red blue yellow cyan white purple\n\
    enter                     对[参数1]问题进行配置，使接下来的命令按照配置文件解析，quit命令结束\n\
  两个参数：\n\
    SourceProblem, source, s  将问题[值1]按照配置文件(详见下一部分)[值2]进行配置\n\
  多个参数：\n\
    Func, f                   将所有参数加入禁用函数列表\n\
    Header, h                 将所有参数加入禁用头文件列表\n\
  示例配置：\n\
    ./judgerc：\n\
      h(fstream)\n\
      f(folk)\n\
      source(aplusb,config)\n\
    ./config:\n\
      time=2.0\n\
      memo=64000\n\
      score=10\n\
\n\
问题配置文件：\n\
    三个选项，格式同judgerc\n\
    time, t                   时间\n\
    memo, memory, m           内存\n\
    score, s                  单个测试点分数\n\
    rename, ren, r            重命名问题\n\
  示例同上。\n\
\n\
CUI模式：\n"+CUIhelp+"\n\
SPJ说明：\n\
将SPJ编译，放在data对应题目目录下，命名为spj(或者将spj.cpp放在该目录下，将会被自动编译)\n\
SPJ参数：\n\
    - argv[1]: 标准输入文件\n\
    - argv[2]: 选手输出文件\n\
    - argv[3]: 标准输出文件\n\
    - argv[4]: 本测试点满分\n\
    - argv[5]: 分数输出文件(必须创建),仅一行,包含一个非负整数,表示得分.\n\
    - argv[6]: 额外信息文件(可以不创建)\n\
";
		cout << help;
	}
    static void WrongArgument(char *str) {
		printf("judge: 未知的选项或不足的参数 '%s'\n请尝试 \"judge --help\"，以获得更多信息。\n",str);
	}
	const char InvalidHeaderFound[]="非法的头文件";
	const char SourceNotFound[]="找不到源代码。";
	const char InvalidWordFound[]="非法的函数";
	const char InvalidConstFound[]="非法的常量";
	const char Score[]="得分";
	const char Time[]="时间";
	const char Memory[]="内存";
	const string Contestant="选手";
	const string Problem="题目";
	const string Error="错误：";
	const string TotalScore="总分数";
	const string SymbolExplanation="符号说明：";
	const string Rejudge="评测";
	const string View="历史";
	static void PressToContinue() {
		puts("按任意键继续......");
		getch();
	}
}
#endif
