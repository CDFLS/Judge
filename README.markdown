# <center>Judge</center>
一个简单的OI程序评测系统，目前只支持Linux，将来也不会支持Windows。</br>
通过调用time与timeout运行程序，当以root运行时会使用chroot来实现一个简单的沙盒。</br>
拥有一个简单的CUI，结果可导出至CSV文件</br>
可将执行过程重定向到文件(将会显示进度)</br>
</br>
运行make构建，运行make en构建英文版。

# 依赖
- coreutils
- time (/usr/bin/time而非bash保留字)

# 安全性
对于一份代码，首先会删除所有include行(同时检查头文件)，然后对g++ -E -P -DEJUDGE的输出进行字符串搜索(从而防止使用宏定义避免搜索)。然后检测是否以root运行，是则使用chroot。

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
