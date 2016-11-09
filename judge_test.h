void judge_test(int c,char *v[]) {
    system("ls ./source > .ejudge.test.oier");
    system("ls ./data > .ejudge.test.problem");
    char problem[10][512],number=0;
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
    char namelist[30][512];int N=0;
    int score[30][10];
    fclose(fp);
    fp=fopen(".ejudge.test.oier","r");
    while (fgets(oier,500,fp)!=NULL) {
        for (int k=strlen(oier)-1;oier[k]=='\n';k++)
            oier[k]=0;
        sprintf(namelist[N++],"%s",oier);
        for (int i=0;i<number;i++) {
            char tmp[512];
            printf("Testing oier '%s' for problem '%s'\n",oier,problem[i]);
            sprintf(tmp,"cp ./source/%s/%s.cpp ./data/%s/",oier,problem[i],problem[i]);
            system(tmp);
            sprintf(tmp,"cd ./data/%s&&judge&&cd ../../",problem[i]);
            system(tmp);
            sprintf(tmp,"./data/%s/.ejudge.score",problem[i]);
            FILE *tmpf=fopen(tmp,"r");
            fscanf(tmpf,"%d",&score[N-1][i]);
            fclose(tmpf);
        }
    }
    printf("    ");
    for (int i=0;i<number;i++)
        printf("%10s",problem[i]);
    puts("");
    for (int i=0;i<N;i++) {
        printf("%10s:",namelist[i]);
        for (int j=0;j<number;j++)
            printf("%4d",score[i][j]);
        puts("");
    }
    fclose(fp);
}
