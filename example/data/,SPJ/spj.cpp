#include <cstdio>

int main(int argc, char **argv) {
	FILE *fp;
	int ans, out, maxscore;
	fp=fopen(argv[2], "r");
	fscanf(fp, "%d", &out);;
	fclose(fp);
	fp=fopen(argv[3], "r");
	fscanf(fp, "%d", &ans);
	fclose(fp);
	sscanf(argv[4], "%d", &maxscore);
	fp=fopen(argv[5], "w");
	if (ans==out)
		fprintf(fp, "%d", maxscore);
	else
		fprintf(fp ,"0");
	fclose(fp);
	fp=fopen(argv[6], "w");
	if (ans==out)
		fprintf(fp, "又强又劲！");
	else
		fprintf(fp, "读到 %d, 期望 %d", out, ans);
	return 0;
}
