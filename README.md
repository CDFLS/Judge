# <center>Judge</center>
一个简单的OI程序评测系统，目前只支持Linux，将来也不会支持Windows。</br>
拥有一个简单的CUI，结果可导出至CSV文件</br>
需要安装软件包'time'(/usr/bin/time而非shell保留字)</br>
运行 'judge --help' 查看帮助</br>
运行make构建，运行make en构建英文版。
截图:</br>
![demo1](https://github.com/Heptagon196/Judge/blob/pic/demo1.png)
![demo2](https://github.com/Heptagon196/Judge/blob/pic/demo2.png)
![demo3](https://github.com/Heptagon196/Judge/blob/pic/demo3.png)
![demo4](https://github.com/Heptagon196/Judge/blob/pic/demo4.png)
![cui](https://github.com/Heptagon196/Judge/blob/pic/cui.png)

# Todo
- [x] 对命令参数的查错
- [x] 对文件输入输出的检查和支持
- [x] 一次完成对一次考试的评测(读取data、source目录下的多个选手程序和多道题目的测试数据)
- [x] 检查头文件，并可通过命令行添加头文件黑名单
- [x] 设置文件
- [x] CUI
- [x] 调用外部SPJ
- [ ] 更多的配置项(编译器，单个问题时间内存限制修改……)
- [ ] Sandbox

#Help
可查看judge --help或查看example目录。
```
用法：judge [选项]...
评测OI程序，编译当前目录下和source/[选手名]目录下的代码(使用该代码父目录名)，并使用当前目录下和data/[题目名]目录下的数据(自动查找)评测。文件操作自动检测，但仅允许freopen。
编译使用命令:
    g++ -static -lm -s [FILENAME].cpp -o [FILENAME] -DEJUDGE
    gcc -static -lm -s [FILENAME].cpp -o [FILENAME] -DEJUDGE

    -w [STRING]               禁止源文件中出现该字符串
    -w[NUMBER] [STRING]...    禁止源文件中出现以下NUMBER个字符串
    -s [STRING]               禁止源文件中出现该头文件
    -s[NUMBER] [STRING]...    禁止源文件中出现以下NUMBER个头文件
    -t [TIME]                 限定程序运行时间(未指定时为1s)
    -m [MEMORY]               限制程序使用内存(为指定时为128000KB)
    --csv                     输出结果到result.csv，当人数大于一时默认开启
    --nocsv                   禁止输出到result.csv(即使人数大于一)
    -c, --cui                 使用一个简单的CUI管理一次考试
    -h, --help                显示本帮助

程序会依次从~/.judgerc和./judgerc中读取设置，设置文件格式为：[选项]=[值1]|[值2]......目前支持以下选项：
  一个参数：
    background, bg            设置输出AC、WA等的背景色，有以下值可选：black green red blue yellow cyan white purple
    InvalidWordsNumber, iwn   设置InvalidWords选项的值数量，默认为1
    InvalidWords, iw          将接下来的InvalidWordsNumber个字符串加入禁用单词列表，以空格分割
    InvalidHeadersNumber, ihn 设置InvalidHeaders选项的值数量，默认为1
    InvalidHeaders, ih        将接下来的InvalidHeadersNumber个字符串加入禁用单词列表，以空格分割
  两个参数：
    SourceProblem, source, sp 将问题[值1]按照配置文件(详见下一部分)[值2]进行配置
  示例配置：
    ./judgerc：
      ih=fstream
      source=aplusb|config
    ./config:
      time=2.0
      memo=64000
      score=10

问题配置文件：
    三个选项，格式为：[选项]=[值]
    time, t                   时间
    memo, memory, m           内存
    score, s                  单个测试点分数
  示例同上。

CUI模式：
    tab键切换模式(重测和查看记录)，ws上下移动，ad翻页，enter选择,q退出

SPJ说明：
将SPJ编译，放在data对应题目目录下，命名为spj(或者将spj.cpp放在该目录下，将会被自动编译)
SPJ参数：
    - argv[1]: 标准输入文件
    - argv[2]: 选手输出文件
    - argv[3]: 标准输出文件
    - argv[4]: 本测试点满分
    - argv[5]: 分数输出文件(必须创建),仅一行,包含一个非负整数,表示得分.
    - argv[6]: 额外信息文件(可以不创建)
```
