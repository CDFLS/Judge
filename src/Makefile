LDFLAG=--std=c++11 -Wno-write-strings -Dlinux -O2
FLAG=main.cpp -o judge --std=c++11 -Wno-write-strings -Dlinux -O2
CPP=g++
DEPENDENCIES=Bar.o judge_cui.o config_judge.o Conio.o Functions_Linux.o execute.o judge.o
judge:$(DEPENDENCIES)
	$(CPP) $(DEPENDENCIES) $(FLAG)
	sudo make install
static:$(DEPENDENCIES)
	$(CPP) $(DEPENDENCIES) $(FLAG) -static
Bar.o:
	$(CPP) -c $(LDFLAG) Bar.cpp
judge_cui.o:
	$(CPP) -c $(LDFLAG) judge_cui.cpp
config_judge.o:
	$(CPP) -c $(LDFLAG) config_judge.cpp
Conio.o:
	$(CPP) -c $(LDFLAG) Conio.cpp
Functions_Linux.o:
	$(CPP) -c $(LDFLAG) Functions_Linux.cpp
execute.o:
	$(CPP) -c $(LDFLAG) sandbox/execute.c -o execute.o
judge.o:
	$(CPP) -c $(LDFLAG) judge.cpp

clean:
	rm *.o
install:
	cp ./judge /usr/bin/judge
debug:$(DEPENDENCIES)
	$(CPP) $(DEPENDENCIES) $(FLAG) -g -DDEBUG
en:$(DEPENDENCIES)
	$(CPP) $(DEPENDENCIES) $(FLAG) -DEN
	sudo make install
