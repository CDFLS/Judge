#include <iostream>
#include <cstdlib>
using namespace std;
int main() {
	int a,b;
	cin >> a >> b;
	cerr << (a%b+a|b);
	return 0;
}
