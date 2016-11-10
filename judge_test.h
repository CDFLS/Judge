int number=0;
struct Judge_Result {
    char name[512];
    int score[10];
    void sumup() {
        score[0]=0;
        for (int i=1;i<=number;i++)
            score[0]+=score[i];
    }
    bool operator < (Judge_Result x) {
        for (int i=0;i<=number;i++)
            if (score[i]!=x.score[i])
                return score[i]>x.score[i];
        return false;
    }
}List[30];
void judge_test(int c,char *v[]) {
    system("ls ./source > .ejudge.test.oier");
    system("ls ./data > .ejudge.test.problem");
    char problem[10][512];
    FILE * fp=fopen(".ejudge.test.problem","r");
    {
        int i=0;
        while (fgets(problem[i],500,fp)!=NULL) {
            for (int k=strlen(problem[i])-1;problem[i][k]=='\n';k++)
                problem[i][k]=0;
            i++;
        }
        number=i;
    }
    char oier[512];
    int N=0;
    int score[30][10];
    fclose(fp);
    fp=fopen(".ejudge.test.oier","r");
    while (fgets(oier,500,fp)!=NULL) {
        for (int k=strlen(oier)-1;oier[k]=='\n';k++)
            oier[k]=0;
        sprintf(List[N++].name,"%s",oier);
        for (int i=0;i<number;i++) {
            char tmp[512];
            printf("Testing '%s' for problem '%s'\n",oier,problem[i]);
            sprintf(tmp,"cp ./source/%s/%s.cpp ./data/%s/ 2>/dev/null",oier,problem[i],problem[i]);
            system(tmp);
            sprintf(tmp,"cd ./data/%s&&judge",problem[i]);
            List[N-1].score[i+1]=WEXITSTATUS(system(tmp));
            system("cd ../..");
            sprintf(tmp,"rm ./data/%s/*.cpp 2>/dev/null",problem[i]);
            system(tmp);
        }
        List[N-1].sumup();
    }
    std::sort(List,List+N);
    for (int i=0;i<number;i++)
        printf("%s",problem[i]);
    puts("");
    for (int i=0;i<N;i++) {
        printf("    %s:",List[i].name);
        for (int j=0;j<number;j++)
            printf("    %d",List[i].score[j+1]);
        puts("");
    }
    fclose(fp);
}
