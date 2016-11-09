void HighLight() {//高亮显示
    printf("\033[1m");
}

inline void ClearColor() {//清除颜色
    printf("\033[0m");
}

inline void ClearFile() {//清理文件
    system("rm ./.ejudge.* 2>/dev/null");
}

void InitFile() {
    system("ls|grep .in > .ejudge.input");
}

void GetName(char *name) {//获取当前目录名
    system("basename $PWD > .ejudge.tmp");
    FILE *fp=fopen(".ejudge.tmp","r");
    fscanf(fp,"%s",name);
    for (int i=strlen(name)-1;i>=0;i--)
        if ((name[i]=='\n')||(name[i]=='\r'))
            name[i]=0;
        else
            return ;
}

