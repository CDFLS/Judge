FLAG=--std=c++11 -Wno-write-strings -Dlinux -O2
ifdef MAKE_EN
	FLAG += -DEN
endif
ifdef MAKE_STATIC
	FLAG += -static
endif
CPP=g++
DEPENDENCIES=main.o Bar.o judge_cui.o ConfigJudge.o Conio.o Functions_Linux.o execute.o judge.o TrieTree.o ConfGen.o
judge:$(DEPENDENCIES)
	$(CPP) $(DEPENDENCIES) $(FLAG) -o judge
	sudo make install

main.o:
	$(CPP) -c $(FLAG) main.cpp
Bar.o:
	$(CPP) -c $(FLAG) Bar.cpp
judge_cui.o:
	$(CPP) -c $(FLAG) judge_cui.cpp
ConfigJudge.o:
	$(CPP) -c $(FLAG) ConfigJudge.cpp
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
ConfGen.o:
	$(CPP) -c $(FLAG) ConfGen.cpp

clean:
	rm *.o
install:
	cp ./judge /usr/bin/judge
debug:$(DEPENDENCIES)
	$(CPP) $(DEPENDENCIES) $(FLAG) -g -DDEBUG
en:
	env MAKE_EN=moha make
static:
	env MAKE_STATIC=moha make
