#ifndef LANG
#define LANG
#include "../judge.h"
#include "../ConverttoString.h"
#include <sstream>
namespace Context {
	const string help=
"Usage:judge [Options]...\n\
Judge OI contest. Compile sources in directory ./ and source/[Contestant name], and use the data in directory ./ and data/[Problem name] automatically to judge. Support stdin and freopen only.\n\
It will use this command to compile: g++ [FILENAME].cpp -o [FILENAME] -DEJUDGE\n\
\n\
    -w [STRING]               Forbidden words in sources\n\
    -w[NUMBER] [STRING]...    [NUMBER] forbidden words in sources\n\
    -s [STRING]               Forbidden headers in sources\n\
    -s[NUMBER] [STRING]...    [NUMBER] forbidden headers in sources\n\
    -t [TIME]                 Limit time (" + CTS::DoubleToString(JudgeSettings::Default_timelimit) + "s by default)\n\
    -m [MEMORY]               Limit memory (" + CTS::IntToString(JudgeSettings::Default_memorylimit) + "KB by default)\n\
    --csv                     Output result to result.csv after judging\n\
    -c, --cui                 Use a simple CUI to manage a contest\n\
    -h, --help                Print this help message and exit\n\
\n\
This program will read settings from ~/.judgerc and ./judgerc. The format is :[Option]=[Value]. Options below are supported so far:\n\
    background, bg            Set the background when print WA,AC,etc. :black green red blue yellow cyan white purple\n\
    InvalidWordsNumber, iwn   Set the number of values in option 'InvalidWords'\n\
    InvalidWords, iw          Add [InvalidWordsNumber] strings to the list of forbidden words, seperated by spaces\n\
    InvalidHeadersNumber, ihn Set the number of values in option 'InvalidHeaders'\n\
    InvalidHeaders, ih        Add [InvalidHeadersNumber] strings to the list of forbidden headers, seperated by spaces\n\
CUI mode:\n\
    tab to switch(rejudge or view history), 'w''s' to choose, 'a''d' to turn page, enter to confirm, 'q' to quit\n\
";
    static void WrongArgument(char *str) {
		printf("judge: Unknown argument '%s'\nMore info with 'judge -h'\n",str);
	}
	const char InvalidHeaderFound[]="Invalid header";
	const char SourceNotFound[]="source not found.";
	const char InvalidWordFound[]="Invalid word";
	const char Score[]="Score";
	const char Time[]="Time";
	const char Memory[]="Memory";
	const string Contestant="Contestant";
	const string Problem="Problem";
	const string Error="Error:";
	const string TotalScore="Score";
	const string SymbolExplanation="Key:";
	const string Rejudge="Rejudge";
	const string View="History";
	static void PressToContinue() {
		puts("Press any key to conyinue ......");
		getch();
	}
}
#endif
