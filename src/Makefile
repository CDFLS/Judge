FLAG=judge.cpp judge_cui.cpp main.cpp sandbox/execute.c -o judge --std=c++11 -lncurses -Wno-write-strings
CPP=g++
DEPENDENCIES=
compile:$(DEPENDENCIES)
	$(CPP) $(FLAG) -O2
	sudo make install
clean:
	rm *.o
install:
	cp ./judge /usr/bin/judge
debug:
	$(CPP) $(FLAG) -g -DDEBUG
en:$(DEPENDENCIES)
	$(CPP) $(FLAG) -O2 -DEN
	sudo make install
