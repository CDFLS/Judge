FLAG=judge.cpp judge_cui.cpp main.cpp -o judge --std=c++11
DEPENDENCIES=/usr/bin/time /usr/bin/timeout
CPP=g++
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
