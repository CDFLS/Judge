# <center>Judge</center>
一个简单的OI程序评测系统，目前只支持Linux，将来也不会支持Windows。</br>
通过调用time与timeout运行程序，当以root运行时会使用chroot来实现一个简单的沙盒。</br>
拥有一个简单的CUI，结果可导出至CSV文件</br>
可将执行过程重定向到文件(将会显示进度)</br>
</br>
运行make构建，运行make en构建英文版。

# 依赖
- coreutils

# 安全性
对于一份代码，首先会在副本中删除所有include行(同时检查头文件)，然后对g++ -E -P -DEJUDGE的输出进行字符串搜索(从而防止使用宏定义避免搜索)。然后检测是否以root运行，是则使用chroot。

# 截图
![demo1](https://github.com/Heptagon196/Judge/blob/pic/demo1.png)
![demo2](https://github.com/Heptagon196/Judge/blob/pic/demo2.png)
![demo3](https://github.com/Heptagon196/Judge/blob/pic/demo3.png)
![demo5](https://github.com/Heptagon196/Judge/blob/pic/demo5.png)
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
- [x] Sandbox
- [x] 更多的配置项
- [x] define解析

# Help
可查看judge --help或查看example目录。
```
用法：judge [选项]...
评测OI程序，编译当前目录下和source/[选手名]目录下的代码(使用该代码父目录名)，并使用当前目录下和data/[题目名]目录下的数据(自动查找)评测。文件操作自动检测(仅检测freopen)。
当以root权限运行judge时，将调用chroot实现一个简单的沙盒。
代码检测仅检测C与C++
编译使用命令:
timeout 10s g++ -static -std=c++11 -s -O2 -lm -DEJUDGE %s -o Exec/%s
timeout 10s gcc -static -std=c99 -fno-asm -s -O2 -lm -DEJUDGE %s -o Exec/%s
timeout 10s fpc -Xs -Sgic -O2 -dEJUDGE %s -oExec/%s
judge python %s Exec/%s
judge python2 %s Exec/%s

    -f [STRING]               禁止源文件中出现该函数
    -f[NUMBER] [STRING]...    禁止源文件中出现以下NUMBER个函数
    -h [STRING]               禁止源文件中出现该头文件
    -h[NUMBER] [STRING]...    禁止源文件中出现以下NUMBER个头文件
    -t [TIME]                 限定程序运行时间(未指定时为1s)
    -m [MEMORY]               限制程序使用内存(为指定时为128000KB)
    --csv                     输出结果到result.csv，当人数大于一时默认开启
    --nocsv                   禁止输出到result.csv(即使人数大于一)
    -c, --cui                 使用一个简单的CUI管理一次考试
    --help                    显示本帮助

全局配置文件：
程序会依次从~/.judgerc和./judgerc中读取设置，设置文件格式为：[选项]([参数1],[参数2]......)
当参数为字符串且参数不定时，会启用转义符，如"\,"表示","，"\("表示"("目前支持以下选项：
  无参数：
    quit                      回到全局配置编辑模式(在enter命令结束时使用)
  一个参数：
    background, bg            设置输出AC、WA等的背景色，有以下值可选：black green red blue yellow cyan white purple
    FileIO, file              强制使用或不使用文件输入输出(默认情况下检测freopen)，值：true false
    enter                     对[参数1]问题进行配置，使接下来的命令按照配置文件解析，quit命令结束
  两个参数：
    SourceProblem, source, s  将问题[值1]按照配置文件(详见下一部分)[值2]进行配置
  多个参数：
    Func, f                   将所有参数加入禁用函数列表
    Header, h                 将所有参数加入禁用头文件列表
  示例配置：
    ./judgerc：
      h(fstream)
      f(folk)
      source(aplusb,config)
      file(true)
    ./config:
      time(2.0)
      memo(64000)
      score(10)

问题配置文件：
    三个选项，格式同judgerc
    time, t                   时间
    memo, memory, m           内存
    score, s                  单个测试点分数
    rename, ren, r            重命名问题
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
