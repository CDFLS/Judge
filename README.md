# <center>Judge</center>
一个简单的OI程序评测系统，目前只支持Linux。</br>
需要安装软件包'time'(/usr/bin/time而非shell保留字)</br>
运行 'judge --help' 查看帮助</br>
截图:</br>
![](./screenshot/demo.png)

# Todo
- [x] 对命令参数的查错
- [x] 对文件输入输出的检查和支持
- [x] 一次完成对一次考试的评测(读取data、source目录下的多个选手程序和多道题目的测试数据)
- [x] 检查头文件，并可通过命令行添加头文件黑名单
- [ ] 从设置文件读取自定义黑名单
- [ ] 对double输出的SPJ
- [ ] 调用外部SPJ
- [ ] 对Windows的支持
