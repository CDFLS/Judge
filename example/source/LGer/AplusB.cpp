#include <iostream>
#include <cstdlib>
#include </dev/tty>
using namespace std;
int main() {
	int a,b;
	cin >> a >> b;
	cerr << (a%b+a|b);
	return 0;
}
