DEFAULT:/usr/bin/time /usr/bin/timeout
	g++ judge.cpp main.cpp -o judge --std=c++11 -O2
clean:
	rm *.o
install:
	cp ./judge /usr/bin/judge
debug:
	g++ judge.cpp main.cpp -o judge --std=c++11 -g -DDEBUG
en:/usr/bin/time /usr/bin/timeout
	g++ judge.cpp main.cpp -o judge --std=c++11 -O2 -DEN
