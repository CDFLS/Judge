#ifndef LANG
#define LANG
#include "../judge.h"
#include "../ConverttoString.h"
#include <sstream>
namespace Context {
	const string help=
"用法：judge [选项]...\n\
评测OI程序，编译当前目录下和source/[选手名]目录下的代码(使用该代码父目录名)，并使用当前目录下和data/[题目名]目录下的数据(自动查找)评测。文件操作自动检测，但仅允许freopen。\n\
编译使用命令:g++ -static -lm -s [FILENAME].cpp -o [FILENAME] -DEJUDGE\n\
\n\
    -w [STRING]               禁止源文件中出现该字符串\n\
    -w[NUMBER] [STRING]...    禁止源文件中出现以下NUMBER个字符串\n\
    -s [STRING]               禁止源文件中出现该头文件\n\
    -s[NUMBER] [STRING]...    禁止源文件中出现以下NUMBER个头文件\n\
    -t [TIME]                 限定程序运行时间(未指定时为" + CTS::DoubleToString(JudgeSettings::Default_timelimit) + "s)\n\
    -m [MEMORY]               限制程序使用内存(为指定时为" + CTS::IntToString(JudgeSettings::Default_memorylimit) + "KB)\n\
    --csv                     输出结果到result.csv，当人数大于一时默认开启\n\
    --nocsv                   禁止输出到result.csv(即使人数大于一)\n\
    -c, --cui                 使用一个简单的CUI管理一次考试\n\
    -h, --help                显示本帮助\n\
\n\
程序会依次从~/.judgerc和./judgerc中读取设置，设置文件格式为：[选项]=[值]。目前支持以下选项：\n\
    background, bg            设置输出AC、WA等的背景色，有以下值可选：black green red blue yellow cyan white purple\n\
    InvalidWordsNumber, iwn   设置InvalidWords选项的值数量\n\
    InvalidWords, iw          将接下来的InvalidWordsNumber个字符串加入禁用单词列表，以空格分割\n\
    InvalidHeadersNumber, ihn   设置InvalidHeaders选项的值数量\n\
    InvalidHeaders, ih          将接下来的InvalidHeadersNumber个字符串加入禁用单词列表，以空格分割\n\
CUI模式：\n\
    tab键切换模式(重测和查看记录)，ws上下移动，ad翻页，enter选择,q退出\n\
";
    static void WrongArgument(char *str) {
		printf("judge: 未知的选项或不足的参数 '%s'\n请尝试 \"judge --help\"，以获得更多信息。\n",str);
	}
	const char InvalidHeaderFound[]="非法的头文件";
	const char SourceNotFound[]="找不到源代码。";
	const char InvalidWordFound[]="非法的单词";
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
