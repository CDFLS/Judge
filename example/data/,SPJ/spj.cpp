#include <cstdio>
#include <cstdlib>
int main(int argc,char **argv) {
	int ans,out;
	FILE *fp=fopen(argv[3],"r");
	fscanf(fp,"%d",&ans);
	fclose(fp);
	fopen(argv[2],"r");
	fscanf(fp,"%d",&out);
	fclose(fp);
	int maxscore=atoi(argv[4]);
	if (ans==out) {
		fp=fopen(argv[5],"w");
		fprintf(fp,"%d",maxscore);
		fclose(fp);
		fp=fopen(argv[6],"w");
		fprintf(fp,"又强又劲！\n");
		fclose(fp);
	} else {
		fp=fopen(argv[5],"w");
		fprintf(fp,"0");
		fclose(fp);
		fp=fopen(argv[6],"w");
		fprintf(fp,"%d found but %d expected\n",out,ans);
		fclose(fp);
	}
	return 0;
}
