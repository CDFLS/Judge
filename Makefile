DEFAULT:
	g++ judge.cpp -o judge
install:
	cp ./judge /usr/bin/judge
uninstall:
	rm /usr/bin/judge
