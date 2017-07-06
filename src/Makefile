FLAG=--std=c++11 -Wno-write-strings -Dlinux -g
ifdef MAKE_EN
	FLAG += -DEN
endif
CPP=g++
DEPENDENCIES=Bar.o judge_cui.o config_judge.o Conio.o Functions_Linux.o execute.o judge.o TrieTree.o main.o
judge:$(DEPENDENCIES)
	$(CPP) $(DEPENDENCIES) $(FLAG) -o judge
	sudo make install
static:$(DEPENDENCIES)
	$(CPP) $(DEPENDENCIES) $(FLAG) -static
Bar.o:
	$(CPP) -c $(FLAG) Bar.cpp
judge_cui.o:
	$(CPP) -c $(FLAG) judge_cui.cpp
config_judge.o:
	$(CPP) -c $(FLAG) config_judge.cpp
Conio.o:
	$(CPP) -c $(FLAG) Conio.cpp
Functions_Linux.o:
	$(CPP) -c $(FLAG) Functions_Linux.cpp
execute.o:
	$(CPP) -c $(FLAG) sandbox/execute.c -o execute.o
judge.o:
	$(CPP) -c $(FLAG) judge.cpp
TrieTree.o:
	$(CPP) -c $(FLAG) TrieTree.cpp
main.o:
	$(CPP) -c $(FLAG) main.cpp

clean:
	rm *.o
install:
	cp ./judge /usr/bin/judge
debug:$(DEPENDENCIES)
	$(CPP) $(DEPENDENCIES) $(FLAG) -g -DDEBUG
en:
	env MAKE_EN=moha make
