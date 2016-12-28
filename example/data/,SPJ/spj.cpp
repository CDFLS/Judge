#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;
int main(int argc,char *argv[]) {
	ifstream fin;
	ofstream fout;
	int ans,out;
	fin.open(argv[3]);
	fin >> ans;
	fin.close();
	fin.open(argv[2]);
	fin >> out;
	fin.close();
	fout.open(argv[5]);
	fout << ((ans==out)?20:0);
	fout.close();
	fout.open(argv[6]);
	if (ans==out)
		fout << "又强又劲！";
	else
		fout << "Read " << out << " but " << ans << " expected" << endl;
	fout.close();
	return 0;
}
